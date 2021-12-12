#pragma once

#include <fstream>
#include <iostream>

#include <google/protobuf/stubs/status.h>
#include <google/protobuf/util/json_util.h>

namespace proto_util = google::protobuf::util;

namespace platypus
{

//==============================================================================
//	typedefs
//==============================================================================
using JsonPrintOptions = proto_util::JsonPrintOptions;
using JsonParseOptions = proto_util::JsonParseOptions;

//==============================================================================
//	exceptions
//==============================================================================
class PlatypusProtoException: public std::exception {
public:
	PlatypusProtoException(const std::string& message)
	: std::exception(message.c_str())
	{ }

	explicit PlatypusProtoException(const char* message)
	: std::exception(message)
	{ }
};

class ProtoParsingError: public PlatypusProtoException {
public:
	ProtoParsingError(const std::string& message)
	: PlatypusProtoException(message)
	{ }

	explicit ProtoParsingError(const char* message)
	: PlatypusProtoException(message)
	{ }
};

class ProtoFileParsingError: public ProtoParsingError {
public:
	ProtoFileParsingError(std::string filename, const std::string& message)
	: ProtoParsingError(message), _filename(std::move(filename))
	{ }

	explicit ProtoFileParsingError(std::string filename, const char* message)
	: ProtoParsingError(message), _filename(std::move(filename))
	{ }

	explicit ProtoFileParsingError(const char* filename, const std::string& message)
	: ProtoParsingError(message), _filename(filename)
	{ }

	explicit ProtoFileParsingError(const char* filename, const char* message)
	: ProtoParsingError(message), _filename(filename)
	{ }

	std::string _filename;
};

//==============================================================================
inline JsonPrintOptions defaultJsonPrintOptions()
{
	JsonPrintOptions options;
	options.add_whitespace = true;
	options.always_print_primitive_fields = true;
	return options;
}

//==============================================================================
template <class T>
inline std::string toJson(T protobuf, JsonPrintOptions options = defaultJsonPrintOptions())
{
	std::string json;
	proto_util::Status status = proto_util::MessageToJsonString(protobuf, &json, options);

	if (!status.ok())
		throw ProtoParsingError(status.message().as_string().c_str());

	return json;
}

//==============================================================================
inline JsonParseOptions defaultJsonParseOptions()
{
	JsonParseOptions options;
	options.case_insensitive_enum_parsing = true;
	return options;
}

//==============================================================================
template <class T>
inline T toProto(std::string json, JsonParseOptions options = defaultJsonParseOptions())
{
	T message;
	proto_util::Status status = proto_util::JsonStringToMessage(json, &message, options);

	if (!status.ok())
		throw ProtoParsingError(status.message().as_string().c_str());

	return message;
}

//==============================================================================
template <class T>
inline T readProtoFile(std::string filepath)
{
	try
	{
		std::ifstream t(filepath);
		t.seekg(0, std::ios::end);
		size_t size = static_cast<size_t>(t.tellg());
		std::string buffer(size, ' ');
		t.seekg(0);
		t.read(&buffer[0], size);

		return toProto<T>(buffer);
	}
	catch (std::exception& e)
	{
		throw ProtoFileParsingError(filepath, e.what());
	}
}

}
