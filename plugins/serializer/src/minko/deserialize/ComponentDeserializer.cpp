/*
Copyright (c) 2013 Aerys

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "minko/deserialize/ComponentDeserializer.hpp"
#include "minko/component/Transform.hpp"
#include "minko/component/PerspectiveCamera.hpp"
#include "minko/component/AmbientLight.hpp"
#include "minko/component/DirectionalLight.hpp"
#include "minko/component/PointLight.hpp"
#include "minko/component/SpotLight.hpp"
#include "minko/component/Surface.hpp"
#include "minko/file/AssetLibrary.hpp"
#include "minko/component/Renderer.hpp"
#include "msgpack.hpp"
#include "minko/deserialize/TypeDeserializer.hpp"
#include "minko/file/Dependency.hpp"
#include "minko/material/Material.hpp"

using namespace minko;
using namespace minko::deserialize;

std::shared_ptr<component::AbstractComponent>
ComponentDeserializer::deserializeTransform(std::string							serializedTransformData,
											std::shared_ptr<file::AssetLibrary>	assetLibrary,
											std::shared_ptr<file::Dependency>	dependencies)
{
	msgpack::zone									mempool;
	msgpack::object									deserialized;
	msgpack::type::tuple<uint, std::string>	dst;

	msgpack::unpack(serializedTransformData.data(), serializedTransformData.size() - 1, NULL, &mempool, &deserialized);
	deserialized.convert(&dst);

	math::Matrix4x4::Ptr transformMatrix = Any::cast<math::Matrix4x4::Ptr>(deserialize::TypeDeserializer::deserializeMatrix4x4(dst));
	
	return component::Transform::create(transformMatrix);
}

std::shared_ptr<component::AbstractComponent>
ComponentDeserializer::deserializeProjectionCamera(std::string							serializedCameraData,
												   std::shared_ptr<file::AssetLibrary>	assetLibrary,
												   std::shared_ptr<file::Dependency>	dependencies)
{
	msgpack::zone								mempool;
	msgpack::object								deserialized;
	msgpack::type::tuple<std::vector<float>>	dst;

	msgpack::unpack(serializedCameraData.data(), serializedCameraData.size() - 1, NULL, &mempool, &deserialized);
	deserialized.convert(&dst);

	return component::PerspectiveCamera::create(dst.a0[0], dst.a0[1], dst.a0[2], dst.a0[3]);
}

std::shared_ptr<component::AbstractComponent>
ComponentDeserializer::deserializeAmbientLight(std::string							serializedAmbientLight,
											   std::shared_ptr<file::AssetLibrary>	assetLibrary,
											   std::shared_ptr<file::Dependency>	dependencies)
{
	msgpack::zone										mempool;
	msgpack::object										deserialized;
	msgpack::type::tuple<float, float, float, float>	dst;
	std::shared_ptr<component::AmbientLight>			ambientLight = component::AmbientLight::create();
	
	msgpack::unpack(serializedAmbientLight.data(), serializedAmbientLight.size() - 1, NULL, &mempool, &deserialized);
	deserialized.convert(&dst);

	ambientLight->ambient(dst.a0);
	ambientLight->color()->setTo(dst.a1, dst.a2, dst.a3);

	return ambientLight;
}

std::shared_ptr<component::AbstractComponent>
ComponentDeserializer::deserializeDirectionalLight(std::string							serializedDirectionalLight,
												   std::shared_ptr<file::AssetLibrary>	assetLibrary,
												   std::shared_ptr<file::Dependency>	dependencies)
{
	msgpack::zone											mempool;
	msgpack::object											deserialized;
	msgpack::type::tuple<float, float, float, float, float>	dst;
	std::shared_ptr<component::DirectionalLight>			directionalLight = component::DirectionalLight::create();
	
	msgpack::unpack(serializedDirectionalLight.data(), serializedDirectionalLight.size() - 1, NULL, &mempool, &deserialized);
	deserialized.convert(&dst);
	
	directionalLight->diffuse(dst.a0);
	directionalLight->specular(dst.a1);
	directionalLight->color()->setTo(dst.a2, dst.a3, dst.a4);

	return directionalLight;
}

std::shared_ptr<component::AbstractComponent>
ComponentDeserializer::deserializePointLight(std::string							serializedPointLight,
											 std::shared_ptr<file::AssetLibrary>	assetLibrary,
											 std::shared_ptr<file::Dependency>		dependencies)
{
	msgpack::zone													mempool;
	msgpack::object													deserialized;
	msgpack::type::tuple<float, float, float, float, float, float>	dst;
	std::shared_ptr<component::PointLight>							pointLight = component::PointLight::create();
	
	msgpack::unpack(serializedPointLight.data(), serializedPointLight.size() - 1, NULL, &mempool, &deserialized);
	deserialized.convert(&dst);

	pointLight->diffuse(dst.a0);
	pointLight->specular(dst.a1);
	pointLight->attenuationDistance(dst.a2);
	pointLight->color()->setTo(dst.a3, dst.a4, dst.a5);

	return pointLight;
}
		
std::shared_ptr<component::AbstractComponent>
ComponentDeserializer::deserializeSpotLight(std::string							serializedSpotLight,
										    std::shared_ptr<file::AssetLibrary>	assetLibrary,
											std::shared_ptr<file::Dependency>	dependencies)
{
	msgpack::zone																	mempool;
	msgpack::object																	deserialized;
	msgpack::type::tuple<float, float, float, float, float, float, float, float>	dst;
	std::shared_ptr<component::SpotLight>											spotLight = component::SpotLight::create();
	
	msgpack::unpack(serializedSpotLight.data(), serializedSpotLight.size() - 1, NULL, &mempool, &deserialized);
	deserialized.convert(&dst);

	spotLight->diffuse(dst.a0);
	spotLight->specular(dst.a1);
	spotLight->attenuationDistance(dst.a2);
	spotLight->innerConeAngle(dst.a3);
	spotLight->outerConeAngle(dst.a4);
	spotLight->color()->setTo(dst.a5, dst.a6, dst.a7);

	return spotLight;
}

std::shared_ptr<component::AbstractComponent>
ComponentDeserializer::deserializeSurface(std::string							serializedSurface,
										  std::shared_ptr<file::AssetLibrary>	assetLibrary,
										  std::shared_ptr<file::Dependency>		dependencies)
{
	msgpack::zone																		mempool;
	msgpack::object																		deserialized;
	msgpack::type::tuple<unsigned short, unsigned short, unsigned short, std::string>	dst;
	
	msgpack::unpack(serializedSurface.data(), serializedSurface.size() - 1, NULL, &mempool, &deserialized);
	deserialized.convert(&dst);

	std::shared_ptr<component::Surface>	surface = component::Surface::create(
		dependencies->getGeometryReference(dst.a0),
		dependencies->getMaterialReference(dst.a1),
		dependencies->getEffectReference(dst.a2),
		dst.a3);

	return surface;
}

std::shared_ptr<component::AbstractComponent>
ComponentDeserializer::deserializeRenderer(std::string							serializedRenderer,
										   std::shared_ptr<file::AssetLibrary>	assetLibrary,
										   std::shared_ptr<file::Dependency>	dependencies)
{
	msgpack::zone							mempool;
	msgpack::object							deserialized;
	msgpack::type::tuple<unsigned int>		dst;
	std::shared_ptr<component::Renderer>	renderer = component::Renderer::create();
	
	msgpack::unpack(serializedRenderer.data(), serializedRenderer.size() - 1, NULL, &mempool, &deserialized);
	deserialized.convert(&dst);

	renderer->backgroundColor(dst.a0);

	return renderer;
}

