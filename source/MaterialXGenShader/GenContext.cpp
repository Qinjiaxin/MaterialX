//
// TM & (c) 2017 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXGenShader/GenContext.h>
#include <MaterialXGenShader/ShaderGenerator.h>

namespace MaterialX
{

//
// GenContext methods
//

GenContext::GenContext(ShaderGeneratorPtr sg) :
    _sg(sg),
    _identifierIndex(0)
{
    if (!_sg)
    {
        throw ExceptionShaderGenError("GenContext must have a valid shader generator");
    }
    _sg->resetIdentifiers(*this);
}

void GenContext::addIdentifier(const string& name)
{
    if (name.empty())
    {
        throw ExceptionShaderGenError("Cannot add empty string as identifier");
    }
    if (_identifiers.count(name))
    {
        throw ExceptionShaderGenError("Identifier '" + name + "' is already used.");
    }
    _identifiers.insert(name);
    ++_identifierIndex;
}

void GenContext::makeIdentifier(string& name)
{
    name = createValidName(name, '_');
    string id = name;
    while (_identifiers.count(id))
    {
        id = name + std::to_string(++_identifierIndex);
    }
    name = id;
    _identifiers.insert(name);
    ++_identifierIndex;
}

void GenContext::clearIdentifiers()
{
    _identifiers.clear();
    _identifierIndex = 0;
}

void GenContext::addNodeImplementation(const string& name, ShaderNodeImplPtr impl)
{
    _nodeImpls[name] = impl;
}

ShaderNodeImplPtr GenContext::findNodeImplementation(const string& name)
{
    auto it = _nodeImpls.find(name);
    return it != _nodeImpls.end() ? it->second : nullptr;
}

void GenContext::clearNodeImplementations()
{
    _nodeImpls.clear();
}

void GenContext::clearUserData()
{
    _userData.clear();
}

void GenContext::addInputSuffix(const ShaderInput* input, const string& suffix)
{
    _inputSuffix[input] = suffix;
}

void GenContext::removeInputSuffix(const ShaderInput* input)
{
    _inputSuffix.erase(input);
}

void GenContext::getInputSuffix(const ShaderInput* input, string& suffix) const
{
    suffix.clear();
    std::unordered_map<const ShaderInput*, string>::const_iterator iter = _inputSuffix.find(input);
    if (iter != _inputSuffix.end())
    {
        suffix = iter->second;
    }
}

void GenContext::addOutputSuffix(const ShaderOutput* output, const string& suffix)
{
    _outputSuffix[output] = suffix;
}

void GenContext::removeOutputSuffix(const ShaderOutput* output)
{
    _outputSuffix.erase(output);
}

void GenContext::getOutputSuffix(const ShaderOutput* output, string& suffix) const
{
    suffix.clear();
    std::unordered_map<const ShaderOutput*, string>::const_iterator iter = _outputSuffix.find(output);
    if (iter != _outputSuffix.end())
    {
        suffix = iter->second;
    }
}

} // namespace MaterialX
