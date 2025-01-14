//
// TM & (c) 2017 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXGenShader/HwShaderGenerator.h>

#include <MaterialXGenShader/Nodes/HwSourceCodeNode.h>
#include <MaterialXGenShader/Nodes/HwCompoundNode.h>
#include <MaterialXGenShader/GenContext.h>

#include <MaterialXCore/Document.h>
#include <MaterialXCore/Definition.h>

namespace MaterialX
{

namespace HW
{
    const string T_IN_POSITION                    = "$inPosition";
    const string T_IN_NORMAL                      = "$inNormal";
    const string T_IN_TANGENT                     = "$inTangent";
    const string T_IN_BITANGENT                   = "$inBitangent";
    const string T_IN_TEXCOORD                    = "$inTexcoord";
    const string T_IN_COLOR                       = "$inColor";
    const string T_POSITION_WORLD                 = "$positionWorld";
    const string T_NORMAL_WORLD                   = "$normalWorld";
    const string T_TANGENT_WORLD                  = "$tangentWorld";
    const string T_BITANGENT_WORLD                = "$bitangentWorld";
    const string T_POSITION_OBJECT                = "$positionObject";
    const string T_NORMAL_OBJECT                  = "$normalObject";
    const string T_TANGENT_OBJECT                 = "$tangentObject";
    const string T_BITANGENT_OBJECT               = "$bitangentObject";
    const string T_TEXCOORD                       = "$texcoord";
    const string T_COLOR                          = "$color";
    const string T_WORLD_MATRIX                   = "$worldMatrix";
    const string T_WORLD_INVERSE_MATRIX           = "$worldInverseMatrix";
    const string T_WORLD_TRANSPOSE_MATRIX         = "$worldTransposeMatrix";
    const string T_WORLD_INVERSE_TRANSPOSE_MATRIX = "$worldInverseTransposeMatrix";
    const string T_VIEW_MATRIX                    = "$viewMatrix";
    const string T_VIEW_INVERSE_MATRIX            = "$viewInverseMatrix";
    const string T_VIEW_TRANSPOSE_MATRIX          = "$viewTransposeMatrix";
    const string T_VIEW_INVERSE_TRANSPOSE_MATRIX  = "$viewInverseTransposeMatrix";
    const string T_PROJ_MATRIX                    = "$projectionMatrix";
    const string T_PROJ_INVERSE_MATRIX            = "$projectionInverseMatrix";
    const string T_PROJ_TRANSPOSE_MATRIX          = "$projectionTransposeMatrix";
    const string T_PROJ_INVERSE_TRANSPOSE_MATRIX  = "$projectionInverseTransposeMatrix";
    const string T_WORLD_VIEW_MATRIX              = "$worldViewMatrix";
    const string T_VIEW_PROJECTION_MATRIX         = "$viewProjectionMatrix";
    const string T_WORLD_VIEW_PROJECTION_MATRIX   = "$worldViewProjectionMatrix";
    const string T_VIEW_POSITION                  = "$viewPosition";
    const string T_VIEW_DIRECTION                 = "$viewDirection";
    const string T_FRAME                          = "$frame";
    const string T_TIME                           = "$time";
    const string T_GEOMATTR                       = "$geomattr";
    const string T_NUM_ACTIVE_LIGHT_SOURCES       = "$numActiveLightSources";
    const string T_ENV_MATRIX                     = "$envMatrix";
    const string T_ENV_IRRADIANCE                 = "$envIrradiance";
    const string T_ENV_RADIANCE                   = "$envRadiance";
    const string T_ENV_RADIANCE_MIPS              = "$envRadianceMips";
    const string T_ENV_RADIANCE_SAMPLES           = "$envRadianceSamples";
    const string T_VERTEX_DATA_INSTANCE           = "$vd";
    const string T_LIGHT_DATA_INSTANCE            = "$lightData";

    const string IN_POSITION                      = "i_position";
    const string IN_NORMAL                        = "i_normal";
    const string IN_TANGENT                       = "i_tangent";
    const string IN_BITANGENT                     = "i_bitangent";
    const string IN_TEXCOORD                      = "i_texcoord";
    const string IN_COLOR                         = "i_color";
    const string POSITION_WORLD                   = "positionWorld";
    const string NORMAL_WORLD                     = "normalWorld";
    const string TANGENT_WORLD                    = "tangentWorld";
    const string BITANGENT_WORLD                  = "bitangentWorld";
    const string POSITION_OBJECT                  = "positionObject";
    const string NORMAL_OBJECT                    = "normalObject";
    const string TANGENT_OBJECT                   = "tangentObject";
    const string BITANGENT_OBJECT                 = "bitangentObject";
    const string TEXCOORD                         = "texcoord";
    const string COLOR                            = "color";
    const string WORLD_MATRIX                     = "u_worldMatrix";
    const string WORLD_INVERSE_MATRIX             = "u_worldInverseMatrix";
    const string WORLD_TRANSPOSE_MATRIX           = "u_worldTransposeMatrix";
    const string WORLD_INVERSE_TRANSPOSE_MATRIX   = "u_worldInverseTransposeMatrix";
    const string VIEW_MATRIX                      = "u_viewMatrix";
    const string VIEW_INVERSE_MATRIX              = "u_viewInverseMatrix";
    const string VIEW_TRANSPOSE_MATRIX            = "u_viewTransposeMatrix";
    const string VIEW_INVERSE_TRANSPOSE_MATRIX    = "u_viewInverseTransposeMatrix";
    const string PROJ_MATRIX                      = "u_projectionMatrix";
    const string PROJ_INVERSE_MATRIX              = "u_projectionInverseMatrix";
    const string PROJ_TRANSPOSE_MATRIX            = "u_projectionTransposeMatrix";
    const string PROJ_INVERSE_TRANSPOSE_MATRIX    = "u_projectionInverseTransposeMatrix";
    const string WORLD_VIEW_MATRIX                = "u_worldViewMatrix";
    const string VIEW_PROJECTION_MATRIX           = "u_viewProjectionMatrix";
    const string WORLD_VIEW_PROJECTION_MATRIX     = "u_worldViewProjectionMatrix";
    const string VIEW_POSITION                    = "u_viewPosition";
    const string VIEW_DIRECTION                   = "u_viewDirection";
    const string FRAME                            = "u_frame";
    const string TIME                             = "u_time";
    const string GEOMATTR                         = "u_geomattr";
    const string NUM_ACTIVE_LIGHT_SOURCES         = "u_numActiveLightSources";
    const string ENV_MATRIX                       = "u_envMatrix";
    const string ENV_IRRADIANCE                   = "u_envIrradiance";
    const string ENV_RADIANCE                     = "u_envRadiance";
    const string ENV_RADIANCE_MIPS                = "u_envRadianceMips";
    const string ENV_RADIANCE_SAMPLES             = "u_envRadianceSamples";
    const string VERTEX_DATA_INSTANCE             = "vd";
    const string LIGHT_DATA_INSTANCE              = "u_lightData";

    const string VERTEX_INPUTS                    = "VertexInputs";
    const string VERTEX_DATA                      = "VertexData";
    const string PRIVATE_UNIFORMS                 = "PrivateUniforms";
    const string PUBLIC_UNIFORMS                  = "PublicUniforms";
    const string LIGHT_DATA                       = "LightData";
    const string PIXEL_OUTPUTS                    = "PixelOutputs";
    const string DIR_N                            = "N";
    const string DIR_L                            = "L";
    const string DIR_V                            = "V";
    const string ATTR_TRANSPARENT                 = "transparent";
    const string USER_DATA_CLOSURE_CONTEXT        = "udcc";
    const string USER_DATA_LIGHT_SHADERS          = "udls";
}

namespace Stage
{
    const string VERTEX = "vertex";
}

//
// HwShaderGenerator methods
//

HwShaderGenerator::HwShaderGenerator(SyntaxPtr syntax) :
    ShaderGenerator(syntax)
{
    // Assign default identifiers names for all tokens.
    // Derived generators can override these names.
    _tokenSubstitutions[HW::T_IN_POSITION] = HW::IN_POSITION;
    _tokenSubstitutions[HW::T_IN_NORMAL] = HW::IN_NORMAL;
    _tokenSubstitutions[HW::T_IN_TANGENT] = HW::IN_TANGENT;
    _tokenSubstitutions[HW::T_IN_BITANGENT] = HW::IN_BITANGENT;
    _tokenSubstitutions[HW::T_IN_TEXCOORD] = HW::IN_TEXCOORD;
    _tokenSubstitutions[HW::T_IN_COLOR] = HW::IN_COLOR;
    _tokenSubstitutions[HW::T_POSITION_WORLD] = HW::POSITION_WORLD;
    _tokenSubstitutions[HW::T_NORMAL_WORLD] = HW::NORMAL_WORLD;
    _tokenSubstitutions[HW::T_TANGENT_WORLD] = HW::TANGENT_WORLD;
    _tokenSubstitutions[HW::T_BITANGENT_WORLD] = HW::BITANGENT_WORLD;
    _tokenSubstitutions[HW::T_POSITION_OBJECT] = HW::POSITION_OBJECT;
    _tokenSubstitutions[HW::T_NORMAL_OBJECT] = HW::NORMAL_OBJECT;
    _tokenSubstitutions[HW::T_TANGENT_OBJECT] = HW::TANGENT_OBJECT;
    _tokenSubstitutions[HW::T_BITANGENT_OBJECT] = HW::BITANGENT_OBJECT;
    _tokenSubstitutions[HW::T_TEXCOORD] = HW::TEXCOORD;
    _tokenSubstitutions[HW::T_COLOR] = HW::COLOR;
    _tokenSubstitutions[HW::T_WORLD_MATRIX] = HW::WORLD_MATRIX;
    _tokenSubstitutions[HW::T_WORLD_INVERSE_MATRIX] = HW::WORLD_INVERSE_MATRIX;
    _tokenSubstitutions[HW::T_WORLD_TRANSPOSE_MATRIX] = HW::WORLD_TRANSPOSE_MATRIX;
    _tokenSubstitutions[HW::T_WORLD_INVERSE_TRANSPOSE_MATRIX] = HW::WORLD_INVERSE_TRANSPOSE_MATRIX;
    _tokenSubstitutions[HW::T_VIEW_MATRIX] = HW::VIEW_MATRIX;
    _tokenSubstitutions[HW::T_VIEW_INVERSE_MATRIX] = HW::VIEW_INVERSE_MATRIX;
    _tokenSubstitutions[HW::T_VIEW_TRANSPOSE_MATRIX] = HW::VIEW_TRANSPOSE_MATRIX;
    _tokenSubstitutions[HW::T_VIEW_INVERSE_TRANSPOSE_MATRIX] = HW::VIEW_INVERSE_TRANSPOSE_MATRIX;
    _tokenSubstitutions[HW::T_PROJ_MATRIX] = HW::PROJ_MATRIX;
    _tokenSubstitutions[HW::T_PROJ_INVERSE_MATRIX] = HW::PROJ_INVERSE_MATRIX;
    _tokenSubstitutions[HW::T_PROJ_TRANSPOSE_MATRIX] = HW::PROJ_TRANSPOSE_MATRIX;
    _tokenSubstitutions[HW::T_PROJ_INVERSE_TRANSPOSE_MATRIX] = HW::PROJ_INVERSE_TRANSPOSE_MATRIX;
    _tokenSubstitutions[HW::T_WORLD_VIEW_MATRIX] = HW::WORLD_VIEW_MATRIX;
    _tokenSubstitutions[HW::T_VIEW_PROJECTION_MATRIX] = HW::VIEW_PROJECTION_MATRIX;
    _tokenSubstitutions[HW::T_WORLD_VIEW_PROJECTION_MATRIX] = HW::WORLD_VIEW_PROJECTION_MATRIX;
    _tokenSubstitutions[HW::T_VIEW_POSITION] = HW::VIEW_POSITION;
    _tokenSubstitutions[HW::T_VIEW_DIRECTION] = HW::VIEW_DIRECTION;
    _tokenSubstitutions[HW::T_FRAME] = HW::FRAME;
    _tokenSubstitutions[HW::T_TIME] = HW::TIME;
    _tokenSubstitutions[HW::T_GEOMATTR] = HW::GEOMATTR;
    _tokenSubstitutions[HW::T_NUM_ACTIVE_LIGHT_SOURCES] = HW::NUM_ACTIVE_LIGHT_SOURCES;
    _tokenSubstitutions[HW::T_ENV_MATRIX] = HW::ENV_MATRIX;
    _tokenSubstitutions[HW::T_ENV_IRRADIANCE] = HW::ENV_IRRADIANCE;
    _tokenSubstitutions[HW::T_ENV_RADIANCE] = HW::ENV_RADIANCE;
    _tokenSubstitutions[HW::T_ENV_RADIANCE_MIPS] = HW::ENV_RADIANCE_MIPS;
    _tokenSubstitutions[HW::T_ENV_RADIANCE_SAMPLES] = HW::ENV_RADIANCE_SAMPLES;
    _tokenSubstitutions[HW::T_VERTEX_DATA_INSTANCE] = HW::VERTEX_DATA_INSTANCE;
    _tokenSubstitutions[HW::T_LIGHT_DATA_INSTANCE] = HW::LIGHT_DATA_INSTANCE;

    // Create closure contexts for defining closure functions
    //
    // Reflection context
    _defReflection = HwClosureContext::create(HwClosureContext::REFLECTION);
    _defReflection->setSuffix("_reflection");
    _defReflection->addArgument(Type::VECTOR3, HW::DIR_L);
    _defReflection->addArgument(Type::VECTOR3, HW::DIR_V);
    // Transmission context
    _defTransmission = HwClosureContext::create(HwClosureContext::TRANSMISSION);
    _defTransmission->setSuffix("_transmission");
    _defTransmission->addArgument(Type::VECTOR3, HW::DIR_V);
    // Indirect context
    _defIndirect = HwClosureContext::create(HwClosureContext::INDIRECT);
    _defIndirect->setSuffix("_indirect");
    _defIndirect->addArgument(Type::VECTOR3, HW::DIR_V);
    // Emission context
    _defEmission = HwClosureContext::create(HwClosureContext::EMISSION);
    _defEmission->addArgument(Type::VECTOR3, HW::DIR_N);
    _defEmission->addArgument(Type::VECTOR3, HW::DIR_L);
}

ShaderPtr HwShaderGenerator::createShader(const string& name, ElementPtr element, GenContext& context) const
{
    // Create the root shader graph
    ShaderGraphPtr graph = ShaderGraph::create(nullptr, name, element, context);
    ShaderPtr shader = std::make_shared<Shader>(name, graph);

    // Create vertex stage.
    ShaderStagePtr vs = createStage(Stage::VERTEX, *shader);
    vs->createInputBlock(HW::VERTEX_INPUTS, "i_vs");
    vs->createUniformBlock(HW::PRIVATE_UNIFORMS, "u_prv");
    vs->createUniformBlock(HW::PUBLIC_UNIFORMS, "u_pub");

    // Create required variables for vertex stage
    VariableBlock& vsInputs = vs->getInputBlock(HW::VERTEX_INPUTS);
    vsInputs.add(Type::VECTOR3, HW::T_IN_POSITION);
    VariableBlock& vsPrivateUniforms = vs->getUniformBlock(HW::PRIVATE_UNIFORMS);
    vsPrivateUniforms.add(Type::MATRIX44, HW::T_WORLD_MATRIX);
    vsPrivateUniforms.add(Type::MATRIX44, HW::T_VIEW_PROJECTION_MATRIX);

    // Create pixel stage.
    ShaderStagePtr ps = createStage(Stage::PIXEL, *shader);
    VariableBlockPtr psOutputs = ps->createOutputBlock(HW::PIXEL_OUTPUTS, "o_ps");
    VariableBlockPtr psPrivateUniforms = ps->createUniformBlock(HW::PRIVATE_UNIFORMS, "u_prv");
    VariableBlockPtr psPublicUniforms = ps->createUniformBlock(HW::PUBLIC_UNIFORMS, "u_pub");
    VariableBlockPtr lightData = ps->createUniformBlock(HW::LIGHT_DATA, HW::T_LIGHT_DATA_INSTANCE);
    lightData->add(Type::INTEGER, "type");

    // Add a block for data from vertex to pixel shader.
    addStageConnectorBlock(HW::VERTEX_DATA, HW::T_VERTEX_DATA_INSTANCE, *vs, *ps);

    // Add uniforms for environment lighting if needed.
    bool lighting = graph->hasClassification(ShaderNode::Classification::SHADER | ShaderNode::Classification::SURFACE) ||
                    graph->hasClassification(ShaderNode::Classification::BSDF);
    if (lighting && context.getOptions().hwSpecularEnvironmentMethod != SPECULAR_ENVIRONMENT_NONE)
    {
        // Note: Generation of the rotation matrix using floating point math can result
        // in values which when output can't be consumed by a h/w shader, so
        // just setting explicit values here for now since the matrix is simple.
        // In general the values will need to be "sanitized" for hardware.
        const Matrix44 yRotationPI(-1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, -1, 0,
                                    0, 0, 0, 1);
        psPrivateUniforms->add(Type::MATRIX44, HW::T_ENV_MATRIX, Value::createValue<Matrix44>(yRotationPI));
        psPrivateUniforms->add(Type::FILENAME, HW::T_ENV_IRRADIANCE);
        psPrivateUniforms->add(Type::FILENAME, HW::T_ENV_RADIANCE);
        psPrivateUniforms->add(Type::INTEGER, HW::T_ENV_RADIANCE_MIPS, Value::createValue<int>(1));
        psPrivateUniforms->add(Type::INTEGER, HW::T_ENV_RADIANCE_SAMPLES, Value::createValue<int>(16));
    }

    // Create uniforms for the published graph interface
    for (ShaderGraphInputSocket* inputSocket : graph->getInputSockets())
    {
        // Only for inputs that are connected/used internally,
        // and are editable by users.
        if (!inputSocket->getConnections().empty() && graph->isEditable(*inputSocket))
        {
            psPublicUniforms->add(inputSocket->getSelf());
        }
    }

    // Add the pixel stage output. This needs to be a color4 for rendering,
    // so copy name and variable from the graph output but set type to color4.
    // TODO: Improve this to support multiple outputs and other data types.
    ShaderGraphOutputSocket* outputSocket = graph->getOutputSocket();
    ShaderPort* output = psOutputs->add(Type::COLOR4, outputSocket->getName());
    output->setVariable(outputSocket->getVariable());
    output->setPath(outputSocket->getPath());

    // Create shader variables for all nodes that need this.
    for (ShaderNode* node : graph->getNodes())
    {
        node->getImplementation().createVariables(*node, context, *shader);
    }

    HwLightShadersPtr lightShaders = context.getUserData<HwLightShaders>(HW::USER_DATA_LIGHT_SHADERS);

    // For surface shaders we need light shaders
    if (lightShaders && graph->hasClassification(ShaderNode::Classification::SHADER | ShaderNode::Classification::SURFACE))
    {
        // Create shader variables for all bound light shaders
        for (const auto& it : lightShaders->get())
        {
            ShaderNode* node = it.second.get();
            node->getImplementation().createVariables(*node, context, *shader);
        }
    }

    //
    // For image textures we need to convert filenames into uniforms (texture samplers).
    // Any unconnected filename input on file texture nodes needs to have a corresponding 
    // uniform.
    //

    // Start with top level graphs.
    std::deque<ShaderGraph*> graphQueue = { graph.get() };
    if (lightShaders)
    {
        for (const auto& it : lightShaders->get())
        {
            ShaderNode* node = it.second.get();
            ShaderGraph* lightGraph = node->getImplementation().getGraph();
            if (lightGraph)
            {
                graphQueue.push_back(lightGraph);
            }
        }
    }

    while (!graphQueue.empty())
    {
        ShaderGraph* g = graphQueue.back();
        graphQueue.pop_back();

        for (ShaderNode* node : g->getNodes())
        {
            if (node->hasClassification(ShaderNode::Classification::FILETEXTURE))
            {
                for (ShaderInput* input : node->getInputs())
                {
                    if (!input->getConnection() && input->getType() == Type::FILENAME)
                    {
                        // Create the uniform using the filename type to make this uniform into a texture sampler.
                        ShaderPort* filename = psPublicUniforms->add(Type::FILENAME, input->getVariable(), input->getValue());
                        filename->setPath(input->getPath());

                        // Assing the uniform name to the input value
                        // so we can reference it during code generation.
                        input->setValue(Value::createValue(input->getVariable()));
                    }
                }
            }
            // Push subgraphs on the queue to process these as well.
            ShaderGraph* subgraph = node->getImplementation().getGraph();
            if (subgraph)
            {
                graphQueue.push_back(subgraph);
            }
        }
    }

    if (context.getOptions().hwTransparency)
    {
        // Flag the shader as being transparent.
        shader->setAttribute(HW::ATTR_TRANSPARENT);
    }

    return shader;
}

void HwShaderGenerator::emitFunctionCall(const ShaderNode& node, GenContext& context, ShaderStage& stage, bool checkScope) const
{
    // Omit node if it's only used inside a conditional branch
    if (checkScope && node.referencedConditionally())
    {
        emitComment("Omitted node '" + node.getName() + "'. Only used in conditional node '" + 
                    node.getScopeInfo().conditionalNode->getName() + "'", stage);
    }
    else
    {
        bool match = true;

        // Check if we have a closure context to modify the function call.
        HwClosureContextPtr ccx = context.getUserData<HwClosureContext>(HW::USER_DATA_CLOSURE_CONTEXT);

        if (ccx && node.hasClassification(ShaderNode::Classification::CLOSURE))
        {
            match =
                // For reflection and indirect we don't support pure transmissive closures.
                ((ccx->getType() == HwClosureContext::REFLECTION || ccx->getType() == HwClosureContext::INDIRECT) &&
                    node.hasClassification(ShaderNode::Classification::BSDF) &&
                    !node.hasClassification(ShaderNode::Classification::BSDF_T)) ||
                // For transmissive we don't support pure reflective closures.
                (ccx->getType() == HwClosureContext::TRANSMISSION &&
                    node.hasClassification(ShaderNode::Classification::BSDF) &&
                    !node.hasClassification(ShaderNode::Classification::BSDF_R)) ||
                // For emission we only support emission closures.
                (ccx->getType() == HwClosureContext::EMISSION &&
                    node.hasClassification(ShaderNode::Classification::EDF));
        }

        if (match)
        {
            // A match between closure context and node classification was found.
            // So emit the function call in this context.
            node.getImplementation().emitFunctionCall(node, context, stage);
        }
        else
        {
            // Context and node classification doen't match so just
            // emit the output variable set to default value, in case
            // it is referenced by another nodes in this context.
            emitLineBegin(stage);
            emitOutput(node.getOutput(), true, true, context, stage);
            emitLineEnd(stage);
        }
    }
}

void HwShaderGenerator::emitTextureNodes(const ShaderGraph& graph, GenContext& context, ShaderStage& stage) const
{
    // Emit function calls for all texturing nodes
    bool found = false;
    for (const ShaderNode* node : graph.getNodes())
    {
        if (node->hasClassification(ShaderNode::Classification::TEXTURE) && !node->referencedConditionally())
        {
            emitFunctionCall(*node, context, stage, false);
            found = true;
        }
    }

    if (found)
    {
        emitLineBreak(stage);
    }
}

void HwShaderGenerator::emitBsdfNodes(const ShaderGraph& graph, const ShaderNode& shaderNode, HwClosureContextPtr ccx,
                                      GenContext& context, ShaderStage& stage, string& bsdf) const
{
    bsdf = _syntax->getTypeSyntax(Type::BSDF).getDefaultValue(false);

    context.pushUserData(HW::USER_DATA_CLOSURE_CONTEXT, ccx);

    // Emit function calls for all BSDF nodes used by this surface shader.
    // The last node will hold the final result.
    const ShaderNode* last = nullptr;
    for (const ShaderNode* node : graph.getNodes())
    {
        if (node->hasClassification(ShaderNode::Classification::BSDF) && shaderNode.isUsedClosure(node))
        {
            emitFunctionCall(*node, context, stage, false);
            last = node;
        }
    }
    if (last)
    {
        bsdf = last->getOutput()->getVariable();
    }

    context.popUserData(HW::USER_DATA_CLOSURE_CONTEXT);
}

void HwShaderGenerator::emitEdfNodes(const ShaderGraph& graph, const ShaderNode& shaderNode, HwClosureContextPtr ccx,
                                     GenContext& context, ShaderStage& stage, string& edf) const
{
    edf = _syntax->getTypeSyntax(Type::EDF).getDefaultValue(false);

    context.pushUserData(HW::USER_DATA_CLOSURE_CONTEXT, ccx);

    // Emit function calls for all EDF nodes used by this shader
    // The last node will hold the final result
    const ShaderNode* last = nullptr;
    for (const ShaderNode* node : graph.getNodes())
    {
        if (node->hasClassification(ShaderNode::Classification::EDF) && shaderNode.isUsedClosure(node))
        {
            emitFunctionCall(*node, context, stage, false);
            last = node;
        }
    }
    if (last)
    {
        edf = last->getOutput()->getVariable();
    }

    context.popUserData(HW::USER_DATA_CLOSURE_CONTEXT);
}

void HwShaderGenerator::bindLightShader(const NodeDef& nodeDef, unsigned int lightTypeId, GenContext& context)
{
    if (TypeDesc::get(nodeDef.getType()) != Type::LIGHTSHADER)
    {
        throw ExceptionShaderGenError("Error binding light shader. Given nodedef '" + nodeDef.getName() + "' is not of lightshader type");
    }

    HwLightShadersPtr lightShaders = context.getUserData<HwLightShaders>(HW::USER_DATA_LIGHT_SHADERS);
    if (!lightShaders)
    {
        lightShaders = HwLightShaders::create();
        context.pushUserData(HW::USER_DATA_LIGHT_SHADERS, lightShaders);
    }

    if (lightShaders->get(lightTypeId))
    {
        throw ExceptionShaderGenError("Error binding light shader. Light type id '" + std::to_string(lightTypeId) +
            "' has already been bound");
    }

    ShaderNodePtr shader = ShaderNode::create(nullptr, nodeDef.getNodeString(), nodeDef, context);

    // Check if this is a graph implementation.
    // If so prepend the light struct instance name on all input socket variables,
    // since in generated code these inputs will be members of the light struct.
    ShaderGraph* graph = shader->getImplementation().getGraph();
    if (graph)
    {
        for (ShaderGraphInputSocket* inputSockets : graph->getInputSockets())
        {
            inputSockets->setVariable("light." + inputSockets->getVariable());
        }
    }

    lightShaders->bind(lightTypeId, shader);
}

void HwShaderGenerator::unbindLightShader(unsigned int lightTypeId, GenContext& context)
{
    HwLightShadersPtr lightShaders = context.getUserData<HwLightShaders>(HW::USER_DATA_LIGHT_SHADERS);
    if (lightShaders)
    {
        lightShaders->unbind(lightTypeId);
    }
}

void HwShaderGenerator::unbindLightShaders(GenContext& context)
{
    HwLightShadersPtr lightShaders = context.getUserData<HwLightShaders>(HW::USER_DATA_LIGHT_SHADERS);
    if (lightShaders)
    {
        lightShaders->clear();
    }
}

void HwShaderGenerator::getNodeClosureContexts(const ShaderNode& node, vector<HwClosureContextPtr>& ccx) const
{
    if (node.hasClassification(ShaderNode::Classification::BSDF))
    {
        if (node.hasClassification(ShaderNode::Classification::BSDF_R))
        {
            // A BSDF for reflection only
            ccx.push_back(_defReflection);
            ccx.push_back(_defIndirect);
        }
        else if (node.hasClassification(ShaderNode::Classification::BSDF_T))
        {
            // A BSDF for transmission only
            ccx.push_back(_defTransmission);
        }
        else
        {
            // A general BSDF handling both reflection and transmission
            ccx.push_back(_defReflection);
            ccx.push_back(_defTransmission);
            ccx.push_back(_defIndirect);
        }
    }
    else if (node.hasClassification(ShaderNode::Classification::EDF))
    {
        // An EDF
        ccx.push_back(_defEmission);
    }
}

ShaderNodeImplPtr HwShaderGenerator::createSourceCodeImplementation(const Implementation&) const
{
    // The standard source code implementation
    // is the implementation to use by default
    return HwSourceCodeNode::create();
}

ShaderNodeImplPtr HwShaderGenerator::createCompoundImplementation(const NodeGraph&) const
{
    // The standard compound implementation
    // is the compound implementation to us by default
    return HwCompoundNode::create();
}

} // namespace MaterialX
