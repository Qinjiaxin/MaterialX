//
// TM & (c) 2017 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXGenShader/HwShaderGenerator.h>
#include <MaterialXGenShader/GenContext.h>
#include <MaterialXRender/LightHandler.h>

namespace MaterialX
{

LightHandler::LightHandler()
{
}

LightHandler::~LightHandler()
{
}

void LightHandler::addLightSource(NodePtr node)
{
    _lightSources.push_back(node);
}

void LightHandler::mapNodeDefToIdentiers(const std::vector<NodePtr>& nodes,
                                           std::unordered_map<string, unsigned int>& ids)
{
    unsigned int id = 1;
    for (const auto& node : nodes)
    {
        auto nodedef = node->getNodeDef();
        if (nodedef)
        {
            const string& name = nodedef->getName();
            if (!ids.count(name))
            {
                ids[name] = id++;
            }
        }
    }
}

void LightHandler::findLights(DocumentPtr doc, std::vector<NodePtr>& lights)
{
    lights.clear();
    for (NodePtr node : doc->getNodes())
    {
        const TypeDesc* type = TypeDesc::get(node->getType());
        if (type == Type::LIGHTSHADER)
        {
            lights.push_back(node);
        }
    }
}

void LightHandler::registerLights(DocumentPtr doc, const std::vector<NodePtr>& lights, GenContext& context)
{
    // Clear context light user data which is set when bindLightShader() 
    // is called. This is necessary in case the light types have already been
    // registered.
    HwShaderGenerator::unbindLightShaders(context);

    if (!lights.empty())
    {
        // Create a list of unique nodedefs and ids for them
        mapNodeDefToIdentiers(lights, _lightIdentifierMap);
        for (const auto& id : _lightIdentifierMap)
        {
            NodeDefPtr nodeDef = doc->getNodeDef(id.first);
            if (nodeDef)
            {
                HwShaderGenerator::bindLightShader(*nodeDef, id.second, context);
            }
        }
    }

    // Make sure max light count is large enough
    const unsigned int lightCount = (unsigned int)lights.size();
    if (lightCount > context.getOptions().hwMaxActiveLightSources)
    {
        context.getOptions().hwMaxActiveLightSources = lightCount;
    }
}

} // namespace MaterialX
