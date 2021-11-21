#pragma once

#include <fstream>

#include <google/protobuf/util/json_util.h>

namespace platypus
{

template <class T>
inline std::string toJson(T protobuf)
{
	std::string json;
	google::protobuf::util::JsonOptions options;
	options.add_whitespace = true;
	google::protobuf::util::MessageToJsonString(protobuf, &json, options);
	return json;
}

template <class T>
inline T toProto(std::string json)
{
	T message;
	google::protobuf::util::JsonStringToMessage(json, &message);
	return message;
}

template <class T>
inline T readProtoFile(std::string filepath)
{
	std::ifstream t(filepath);
	t.seekg(0, std::ios::end);
	size_t size = static_cast<size_t>(t.tellg());
	std::string buffer(size, ' ');
	t.seekg(0);
	t.read(&buffer[0], size);

	return toProto<T>(buffer);
}

}
