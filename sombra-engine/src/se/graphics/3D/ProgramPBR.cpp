#include <array>
#include <sstream>
#include <fstream>
#include "se/graphics/3D/ProgramPBR.h"
#include "se/graphics/Shader.h"
#include "se/graphics/Program.h"
#include "se/graphics/3D/Lights.h"
#include "se/graphics/3D/Material.h"

namespace se::graphics {

	bool ProgramPBR::init()
	{
		return createProgram("res/shaders/vertexPBR.glsl", "res/shaders/fragmentPBR.glsl")
			&& addUniforms();
	}


	void ProgramPBR::setMaterial(const Material& material) const
	{
		mProgram->setUniform("uMaterial.pbrMetallicRoughness.baseColorFactor", material.pbrMetallicRoughness.baseColorFactor);

		int useBaseColorTexture = (material.pbrMetallicRoughness.baseColorTexture != nullptr);
		mProgram->setUniform("uMaterial.pbrMetallicRoughness.useBaseColorTexture", useBaseColorTexture);
		if (useBaseColorTexture) {
			mProgram->setUniform("uMaterial.pbrMetallicRoughness.baseColorTexture", TextureUnits::kBaseColor);
			material.pbrMetallicRoughness.baseColorTexture->bind(TextureUnits::kBaseColor);
		}

		mProgram->setUniform("uMaterial.pbrMetallicRoughness.metallicFactor", material.pbrMetallicRoughness.metallicFactor);
		mProgram->setUniform("uMaterial.pbrMetallicRoughness.roughnessFactor", material.pbrMetallicRoughness.roughnessFactor);

		int useMetallicRoughnessTexture = (material.pbrMetallicRoughness.metallicRoughnessTexture != nullptr);
		mProgram->setUniform("uMaterial.pbrMetallicRoughness.useMetallicRoughnessTexture", useMetallicRoughnessTexture);
		if (useMetallicRoughnessTexture) {
			mProgram->setUniform("uMaterial.pbrMetallicRoughness.metallicRoughnessTexture", TextureUnits::kMetallicRoughness);
			material.pbrMetallicRoughness.metallicRoughnessTexture->bind(TextureUnits::kMetallicRoughness);
		}

		int useNormalTexture = (material.normalTexture != nullptr);
		mProgram->setUniform("uMaterial.useNormalTexture", useNormalTexture);
		if (useNormalTexture) {
			mProgram->setUniform("uMaterial.normalTexture", TextureUnits::kNormal);
			material.normalTexture->bind(TextureUnits::kNormal);
			mProgram->setUniform("uMaterial.normalScale", material.normalScale);
		}

		int useOcclusionTexture = (material.occlusionTexture != nullptr);
		mProgram->setUniform("uMaterial.useOcclusionTexture", useOcclusionTexture);
		if (useOcclusionTexture) {
			mProgram->setUniform("uMaterial.occlusionTexture", TextureUnits::kOcclusion);
			material.occlusionTexture->bind(TextureUnits::kOcclusion);
			mProgram->setUniform("uMaterial.occlusionStrength", material.occlusionStrength);
		}

		int useEmissiveTexture = (material.emissiveTexture != nullptr);
		mProgram->setUniform("uMaterial.useEmissiveTexture", useEmissiveTexture);
		if (useEmissiveTexture) {
			mProgram->setUniform("uMaterial.emissiveTexture", TextureUnits::kEmissive);
			material.emissiveTexture->bind(TextureUnits::kEmissive);
		}

		mProgram->setUniform("uMaterial.emissiveFactor", material.emissiveFactor);

		int checkAlphaCutoff = (material.alphaMode == AlphaMode::Mask);
		mProgram->setUniform("uMaterial.checkAlphaCutoff", checkAlphaCutoff);
		if (checkAlphaCutoff) {
			mProgram->setUniform("uMaterial.alphaCutoff", material.alphaCutoff);
		}
	}


	void ProgramPBR::setLights(const std::vector<const ILight*>& lights) const
	{
		int numPointLights = 0;
		std::array<glm::vec3, kMaxPointLights> positions;

		for (const ILight* light : lights) {
			const PointLight* pLight = dynamic_cast<const PointLight*>(light);
			if (pLight && (numPointLights < kMaxPointLights)) {
				mProgram->setUniform(("uPointLights[" + std::to_string(numPointLights) + "].color").c_str(), pLight->color);
				mProgram->setUniform(("uPointLights[" + std::to_string(numPointLights) + "].intensity").c_str(), pLight->intensity);
				mProgram->setUniform(("uPointLights[" + std::to_string(numPointLights) + "].inverseRange").c_str(), pLight->inverseRange);
				positions[numPointLights] = pLight->position;

				numPointLights++;
			}
		}

		mProgram->setUniform("uNumPointLights", numPointLights);
		mProgram->setUniformV("uPointLightsPositions", numPointLights, positions.data());
	}

// Private functions
	bool ProgramPBR::addUniforms()
	{
		if (!Program3D::addUniforms()) {
			return false;
		}

		if (!mProgram->addUniform("uMaterial.pbrMetallicRoughness.baseColorFactor")
			|| !mProgram->addUniform("uMaterial.pbrMetallicRoughness.useBaseColorTexture")
			|| !mProgram->addUniform("uMaterial.pbrMetallicRoughness.baseColorTexture")
			|| !mProgram->addUniform("uMaterial.pbrMetallicRoughness.metallicFactor")
			|| !mProgram->addUniform("uMaterial.pbrMetallicRoughness.roughnessFactor")
			|| !mProgram->addUniform("uMaterial.pbrMetallicRoughness.useMetallicRoughnessTexture")
			|| !mProgram->addUniform("uMaterial.pbrMetallicRoughness.metallicRoughnessTexture")
			|| !mProgram->addUniform("uMaterial.useNormalTexture")
			|| !mProgram->addUniform("uMaterial.normalTexture")
			|| !mProgram->addUniform("uMaterial.normalScale")
			|| !mProgram->addUniform("uMaterial.useOcclusionTexture")
			|| !mProgram->addUniform("uMaterial.occlusionTexture")
			|| !mProgram->addUniform("uMaterial.occlusionStrength")
			|| !mProgram->addUniform("uMaterial.useEmissiveTexture")
			|| !mProgram->addUniform("uMaterial.emissiveTexture")
			|| !mProgram->addUniform("uMaterial.emissiveFactor")
			|| !mProgram->addUniform("uMaterial.checkAlphaCutoff")
			|| !mProgram->addUniform("uMaterial.alphaCutoff")
		) {
			return false;
		}

		if (!mProgram->addUniform("uNumPointLights")) {
			return false;
		}
		for (int i = 0; i < kMaxPointLights; ++i) {
			if (!mProgram->addUniform(("uPointLights[" + std::to_string(i) + "].color").c_str())
				|| !mProgram->addUniform(("uPointLights[" + std::to_string(i) + "].intensity").c_str())
				|| !mProgram->addUniform(("uPointLights[" + std::to_string(i) + "].inverseRange").c_str())
			) {
				return false;
			}
		}
		if (!mProgram->addUniform("uPointLightsPositions")) {
			return false;
		}

		return true;
	}

}
