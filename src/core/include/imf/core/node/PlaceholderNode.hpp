#pragma once

#include <imf/core/DataFlow.hpp>
#include <imf/core/GraphNode.hpp>
#include <imf/core/TypeQualifier.hpp>
#include <imf/core/TypeID.hpp>

#include <any>
#include <optional>

namespace imf::core
{

class PlaceholderNode final : public GraphNode
{
public:
	constexpr static std::string_view operation_type = "Placeholder";

	template<typename T> PlaceholderNode(TypeQualifier qualifier, T&& value);
	PlaceholderNode(TypeQualifier qualifier, TypeID typeId);
	PlaceholderNode(const PlaceholderNode&) = delete;
	PlaceholderNode(PlaceholderNode&&) noexcept = delete;
	PlaceholderNode& operator=(const PlaceholderNode&) = delete;
	PlaceholderNode& operator=(PlaceholderNode&&) noexcept = delete;

	// getters
	std::string_view operationName() const noexcept override;
	iterator_range<const std::string_view*> inputNames() const noexcept override;
	iterator_range<const TypeID*> inputTypes() const noexcept override;
	iterator_range<const std::string_view*> outputNames() const noexcept override;
	iterator_range<const TypeID*> outputTypes() const noexcept override;
	iterator_range<const DataFlow*> outputs() const noexcept override;
	hast_t hash() const noexcept override;

	// setters
	void setInput(const std::string_view& name, const DataFlow& flow) override;

	template<typename T> void setValue(T&& value);

	template<typename... Args> static std::shared_ptr<PlaceholderNode> make_constant(Args&& ...args);
	template<typename... Args> static std::shared_ptr<PlaceholderNode> make_variable(Args&& ...args);
	template<typename... Args> static std::shared_ptr<PlaceholderNode> make(Args&& ...args);

private:
	DataFlow m_output;
	hast_t m_value_hash;
	std::any m_value;
	TypeQualifier m_qualifier;

	mutable std::optional<hast_t> m_hash;
};

template<typename T>
PlaceholderNode::PlaceholderNode(TypeQualifier qualifier, T&& value) :
	m_output(this, TypeID::make<std::decay_t<T>>()),
	m_value_hash(qualifier == TypeQualifier::Constant ? std::hash<std::decay_t<T>>{}(value) : std::hash<unique_id_t>{}(make_unique_id())),
	m_value(std::forward<T>(value)),
	m_qualifier(qualifier)
{
}

template<typename T> void PlaceholderNode::setValue(T&& value)
{
	if (TypeID::make<std::decay_t<T>>() != m_output.dataType())
	{
		throw std::invalid_argument("value type does not match output type");
	}

	if (m_qualifier == TypeQualifier::Constant)
	{
		m_value_hash = std::hash<std::decay_t<T>>{}(value);
	}

	m_value = std::forward<T>(value);
	m_hash.reset();
}

template<typename... Args> std::shared_ptr<PlaceholderNode> PlaceholderNode::make_constant(Args&& ...args)
{
	return std::make_shared<PlaceholderNode>(TypeQualifier::Constant, std::forward<Args>(args)...);
}

template<typename... Args> std::shared_ptr<PlaceholderNode> PlaceholderNode::make_variable(Args&& ...args)
{
	return std::make_shared<PlaceholderNode>(TypeQualifier::Variable, std::forward<Args>(args)...);
}

template<typename... Args> std::shared_ptr<PlaceholderNode> PlaceholderNode::make(Args&& ...args)
{
	return std::make_shared<PlaceholderNode>(std::forward<Args>(args)...);
}

}
