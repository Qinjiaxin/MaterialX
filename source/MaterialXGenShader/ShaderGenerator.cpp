//
// TM & (c) 2017 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXGenShader/ShaderGenerator.h>

#include <MaterialXGenShader/GenContext.h>
#include <MaterialXGenShader/ShaderNodeImpl.h>
#include <MaterialXGenShader/Nodes/CompoundNode.h>
#include <MaterialXGenShader/Nodes/SourceCodeNode.h>
#include <MaterialXGenShader/Util.h>

#include <MaterialXFormat/File.h>

#include <MaterialXCore/Document.h>
#include <MaterialXCore/Node.h>
#include <MaterialXCore/Value.h>

#include <sstream>

namespace MaterialX
{

const string ShaderGenerator::SEMICOLON = ";";
const string ShaderGenerator::COMMA = ",";

//
// ShaderGenerator methods
//

ShaderGenerator::ShaderGenerator(SyntaxPtr syntax) :
     _syntax(syntax)
{
}

void ShaderGenerator::emitScopeBegin(ShaderStage& stage, Syntax::Punctuation punc) const
{
    stage.beginScope(punc);
}

void ShaderGenerator::emitScopeEnd(ShaderStage& stage, bool semicolon, bool newline) const
{
    stage.endScope(semicolon, newline);
}

void ShaderGenerator::emitLineBegin(ShaderStage& stage) const
{
    stage.beginLine();
}

void ShaderGenerator::emitLineEnd(ShaderStage& stage, bool semicolon) const
{
    stage.endLine(semicolon);
}

void ShaderGenerator::emitLineBreak(ShaderStage& stage) const
{
    stage.newLine();
}

void ShaderGenerator::emitString(const string& str, ShaderStage& stage) const
{
    stage.addString(str);
}

void ShaderGenerator::emitLine(const string& str, ShaderStage& stage, bool semicolon) const
{
    stage.addLine(str, semicolon);
}

void ShaderGenerator::emitComment(const string& str, ShaderStage& stage) const
{
    stage.addComment(str);
}

void ShaderGenerator::emitBlock(const string& str, GenContext& context, ShaderStage& stage) const
{
    stage.addBlock(str, context);
}

void ShaderGenerator::emitInclude(const string& file, GenContext& context, ShaderStage& stage) const
{
    stage.addInclude(file, context);
}

void ShaderGenerator::emitFunctionDefinition(const ShaderNode& node, GenContext& context, ShaderStage& stage) const
{
    stage.addFunctionDefinition(node, context);
}

void ShaderGenerator::emitFunctionCall(const ShaderNode& node, GenContext& context, ShaderStage& stage,
                                       bool checkScope) const
{
    // Omit node if it's only used inside a conditional branch
    if (checkScope && node.referencedConditionally())
    {
        emitComment("Omitted node '" + node.getName() + "'. Only used in conditional node '" +
                    node.getScopeInfo().conditionalNode->getName() + "'", stage);
    }
    else
    {
        node.getImplementation().emitFunctionCall(node, context, stage);
    }
}

void ShaderGenerator::emitFunctionDefinitions(const ShaderGraph& graph, GenContext& context, ShaderStage& stage) const
{
    // Emit function definitions for all nodes in the graph.
    for (ShaderNode* node : graph.getNodes())
    {
        emitFunctionDefinition(*node, context, stage);
    }
}

void ShaderGenerator::emitFunctionCalls(const ShaderGraph& graph, GenContext& context, ShaderStage& stage) const
{
    // Emit function calls for all nodes in the graph.
    for (ShaderNode* node : graph.getNodes())
    {
        emitFunctionCall(*node, context, stage);
    }
}

void ShaderGenerator::emitTypeDefinitions(GenContext&, ShaderStage& stage) const
{
    // Emit typedef statements for all data types that have an alias
    for (const auto& syntax : _syntax->getTypeSyntaxes())
    {
        if (!syntax->getTypeAlias().empty())
        {
            stage.addLine("#define " + syntax->getName() + " " + syntax->getTypeAlias(), false);
        }
        if (!syntax->getTypeDefinition().empty())
        {
            stage.addLine(syntax->getTypeDefinition(), false);
        }
    }
    stage.newLine();
}

void ShaderGenerator::emitVariableDeclaration(const ShaderPort* variable, const string& qualifier, 
                                              GenContext&, ShaderStage& stage,
                                              bool assignValue) const
{
    string str = qualifier.empty() ? EMPTY_STRING : qualifier + " ";
    str += _syntax->getTypeName(variable->getType()) + " " + variable->getVariable();

    // If an array we need an array qualifier (suffix) for the variable name
    if (variable->getType()->isArray() && variable->getValue())
    {
        str += _syntax->getArraySuffix(variable->getType(), *variable->getValue());
    }

    if (assignValue)
    {
        const string valueStr = (variable->getValue() ?
            _syntax->getValue(variable->getType(), *variable->getValue(), true) :
            _syntax->getDefaultValue(variable->getType(), true));
        str += valueStr.empty() ? EMPTY_STRING : " = " + valueStr;
    }

    stage.addString(str);
}

void ShaderGenerator::emitVariableDeclarations(const VariableBlock& block, const string& qualifier, const string& separator, 
                                               GenContext& context, ShaderStage& stage,
                                               bool assignValue) const
{
    for (size_t i=0; i<block.size(); ++i)
    {
        emitLineBegin(stage);
        emitVariableDeclaration(block[i], qualifier, context, stage, assignValue);
        emitString(separator, stage);
        emitLineEnd(stage, false);
    }
}

void ShaderGenerator::emitInput(const ShaderInput* input, GenContext& context, ShaderStage& stage) const
{
    stage.addString(getUpstreamResult(input, context));
}

void ShaderGenerator::emitOutput(const ShaderOutput* output, bool includeType, bool assignValue, GenContext& context, ShaderStage& stage) const
{
    stage.addString(includeType ? _syntax->getTypeName(output->getType()) + " " + output->getVariable() : output->getVariable());

    // Look for any additional suffix to append
    string suffix;
    context.getOutputSuffix(output, suffix);
    if (!suffix.empty())
    {
        stage.addString(suffix);
    }

    if (assignValue)
    {
        const string& value = _syntax->getDefaultValue(output->getType());
        if (!value.empty())
        {
            stage.addString(" = " + value);
        }
    }
}

string ShaderGenerator::getUpstreamResult(const ShaderInput* input, GenContext& context) const
{
    if (!input->getConnection())
    {
        return input->getValue() ? _syntax->getValue(input->getType(), *input->getValue()) : _syntax->getDefaultValue(input->getType());
    }

    string variable = input->getConnection()->getVariable();
    if (!input->getChannels().empty())
    {
        variable = _syntax->getSwizzledVariable(variable, input->getConnection()->getType(), input->getChannels(), input->getType());
    }

    // Look for any additional suffix to append
    string suffix;
    context.getInputSuffix(input, suffix);
    if (!suffix.empty())
    {
        variable += suffix;
    }

    return variable;
}

void ShaderGenerator::resetIdentifiers(GenContext& context) const
{
    // Clear any previous identifiers.
    context.clearIdentifiers();

    // Add in the restricted names as taken names.
    for (const auto& name : _syntax->getRestrictedNames())
    {
        context.addIdentifier(name);
    }

    // Add in the token substitution identifiers as taken names
    for (const auto& it : _tokenSubstitutions)
    {
        // Do no add empty token substitutions as identifiers
        if (!it.second.empty())
        {
            context.addIdentifier(it.second);
        }
    }
}

void ShaderGenerator::registerImplementation(const string& name, CreatorFunction<ShaderNodeImpl> creator)
{
    _implFactory.registerClass(name, creator);
}

bool ShaderGenerator::implementationRegistered(const string& name) const
{
    return _implFactory.classRegistered(name);
}

ShaderNodeImplPtr ShaderGenerator::getImplementation(const InterfaceElement& element, GenContext& context) const
{
    const string& name = element.getName();

    // Check if it's created and cached already.
    ShaderNodeImplPtr impl = context.findNodeImplementation(name);
    if (impl)
    {
        return impl;
    }

    if (element.isA<NodeGraph>())
    {
        // Use a compound implementation.
        impl = createCompoundImplementation(static_cast<const NodeGraph&>(element));
    }
    else if (element.isA<Implementation>())
    {
        // Try creating a new in the factory.
        impl = _implFactory.create(name);
        if (!impl)
        {
            // Fall back to the source code implementation.
            impl = createSourceCodeImplementation(static_cast<const Implementation&>(element));
        }
    }
    else
    {
        throw ExceptionShaderGenError("Element '" + name + "' is neither an Implementation nor an NodeGraph");
    }
    impl->initialize(element, context);

    // Cache it.
    context.addNodeImplementation(name, impl);

    return impl;
}

bool ShaderGenerator::remapEnumeration(const ValueElement&, const string&, std::pair<const TypeDesc*, ValuePtr>&) const
{
    return false;
}

namespace
{
    void replace(const StringMap& substitutions, ShaderPort* port)
    {
        string name = port->getName();
        tokenSubstitution(substitutions, name);
        port->setName(name);
        string variable = port->getVariable();
        tokenSubstitution(substitutions, variable);
        port->setVariable(variable);
    }
}

void ShaderGenerator::replaceTokens(const StringMap& substitutions, ShaderStage& stage) const
{
    // Replace tokens in source code
    tokenSubstitution(substitutions, stage._code);

    // Replace tokens on shader interface
    for (size_t i = 0; i < stage._constants.size(); ++i)
    {
        replace(substitutions, stage._constants[i]);
    }
    for (const auto& it : stage._uniforms)
    {
        VariableBlock& uniforms = *it.second;
        for (size_t i = 0; i < uniforms.size(); ++i)
        {
            replace(substitutions, uniforms[i]);
        }
    }
    for (const auto& it : stage._inputs)
    {
        VariableBlock& inputs = *it.second;
        for (size_t i = 0; i < inputs.size(); ++i)
        {
            replace(substitutions, inputs[i]);
        }
    }
    for (const auto& it : stage._outputs)
    {
        VariableBlock& outputs = *it.second;
        for (size_t i = 0; i < outputs.size(); ++i)
        {
            replace(substitutions, outputs[i]);
        }
    }
}

ShaderStagePtr ShaderGenerator::createStage(const string& name, Shader& shader) const
{
    return shader.createStage(name, _syntax);
}

ShaderNodeImplPtr ShaderGenerator::createSourceCodeImplementation(const Implementation&) const
{
    // The standard source code implementation
    // is the implementation to use by default
    return SourceCodeNode::create();
}

ShaderNodeImplPtr ShaderGenerator::createCompoundImplementation(const NodeGraph&) const
{
    // The standard compound implementation
    // is the compound implementation to us by default
    return CompoundNode::create();
}

} // namespace MaterialX
