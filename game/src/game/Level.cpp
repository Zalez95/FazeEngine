#include <numeric>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <AudioFile.h>

#include <se/window/KeyCodes.h>

#include <se/app/graphics/Image.h>
#include <se/app/graphics/RawMesh.h>
#include <se/app/graphics/Camera.h>
#include <se/app/graphics/LightSource.h>
#include <se/app/graphics/Material.h>
#include <se/app/graphics/TextureUtils.h>
#include <se/app/loaders/MeshLoader.h>
#include <se/app/loaders/ImageReader.h>
#include <se/app/loaders/FontReader.h>
#include <se/app/loaders/TerrainLoader.h>
#include <se/app/loaders/SceneReader.h>
#include <se/app/loaders/TechniqueLoader.h>
#include <se/app/EntityDatabase.h>
#include <se/app/CameraSystem.h>
#include <se/app/AudioSystem.h>
#include <se/app/AppRenderer.h>
#include <se/app/TagComponent.h>
#include <se/app/TransformsComponent.h>

#include <se/graphics/Renderer.h>
#include <se/graphics/3D/Mesh.h>
#include <se/graphics/3D/RenderableMesh.h>
#include <se/graphics/core/GraphicsOperations.h>

#include <se/collision/BoundingBox.h>
#include <se/collision/BoundingSphere.h>
#include <se/collision/ConvexPolyhedron.h>
#include <se/collision/CompositeCollider.h>
#include <se/collision/HalfEdgeMeshExt.h>
#include <se/collision/QuickHull.h>
#include <se/collision/HACD.h>

#include <se/physics/RigidBody.h>
#include <se/physics/PhysicsEngine.h>
#include <se/physics/forces/Gravity.h>
#include <se/physics/constraints/DistanceConstraint.h>

#include <se/animation/AnimationEngine.h>

#include <se/audio/Buffer.h>
#include <se/audio/Source.h>

#include <se/utils/Log.h>
#include <se/utils/Repository.h>

#include "Game.h"
#include "Level.h"

namespace game {

	se::collision::HalfEdgeMesh createTestTube1()
	{
		se::collision::HalfEdgeMesh meshData;
		std::array<int, 48> vertexIndices = {
			se::collision::addVertex(meshData, { -0.000000014f, 0.499999761f, -1.0f }),
			se::collision::addVertex(meshData, { -0.000000014f, 0.499999761f, 1.0f }),
			se::collision::addVertex(meshData, { 0.249999970f, 0.433012485f, -1.0f }),
			se::collision::addVertex(meshData, { 0.249999970f, 0.433012485f, 1.0f }),
			se::collision::addVertex(meshData, { 0.433012694f, 0.249999791f, -1.0f }),
			se::collision::addVertex(meshData, { 0.433012694f, 0.249999791f, 1.0f }),
			se::collision::addVertex(meshData, { 0.5f, -0.000000210f, -1.0f }),
			se::collision::addVertex(meshData, { 0.5f, -0.000000210f, 1.0f }),
			se::collision::addVertex(meshData, { 0.433012694f, -0.250000208f, -1.0f }),
			se::collision::addVertex(meshData, { 0.433012694f, -0.250000208f, 1.0f }),
			se::collision::addVertex(meshData, { 0.250000029f, -0.433012902f, -1.0f }),
			se::collision::addVertex(meshData, { 0.250000029f, -0.433012902f, 1.0f }),
			se::collision::addVertex(meshData, { 0.00000006f, -0.500000178f, -1.0f }),
			se::collision::addVertex(meshData, { 0.00000006f, -0.500000178f, 1.0f }),
			se::collision::addVertex(meshData, { -0.249999910f, -0.433012962f, -1.0f }),
			se::collision::addVertex(meshData, { -0.249999910f, -0.433012962f, 1.0f }),
			se::collision::addVertex(meshData, { -0.433012634f, -0.250000357f, -1.0f }),
			se::collision::addVertex(meshData, { -0.433012634f, -0.250000357f, 1.0f }),
			se::collision::addVertex(meshData, { -0.5f, -0.000000421f, -1.0f }),
			se::collision::addVertex(meshData, { -0.5f, -0.000000421f, 1.0f }),
			se::collision::addVertex(meshData, { -0.433012872f, 0.249999567f, -1.0f }),
			se::collision::addVertex(meshData, { -0.433012872f, 0.249999567f, 1.0f }),
			se::collision::addVertex(meshData, { -0.250000327f, 0.433012336f, -1.0f }),
			se::collision::addVertex(meshData, { -0.250000327f, 0.433012336f, 1.0f }),
			se::collision::addVertex(meshData, { 0.0f, 1.0f, -1.0f }),
			se::collision::addVertex(meshData, { 0.0f, 1.0f, 1.0f }),
			se::collision::addVertex(meshData, { 0.5f, 0.866025388f, -1.0f }),
			se::collision::addVertex(meshData, { 0.5f, 0.866025388f, 1.0f }),
			se::collision::addVertex(meshData, { 0.866025447f, 0.499999970f, -1.0f }),
			se::collision::addVertex(meshData, { 0.866025447f, 0.499999970f, 1.0f }),
			se::collision::addVertex(meshData, { 1.0f, -0.000000043f, -1.0f }),
			se::collision::addVertex(meshData, { 1.0f, -0.000000043f, 1.0f }),
			se::collision::addVertex(meshData, { 0.866025388f, -0.500000059f, -1.0f }),
			se::collision::addVertex(meshData, { 0.866025388f, -0.500000059f, 1.0f }),
			se::collision::addVertex(meshData, { 0.500000059f, -0.866025388f, -1.0f }),
			se::collision::addVertex(meshData, { 0.500000059f, -0.866025388f, 1.0f }),
			se::collision::addVertex(meshData, { 0.00000015f, -1.0f, -1.0f }),
			se::collision::addVertex(meshData, { 0.00000015f, -1.0f, 1.0f }),
			se::collision::addVertex(meshData, { -0.499999791f, -0.866025507f, -1.0f }),
			se::collision::addVertex(meshData, { -0.499999791f, -0.866025507f, 1.0f }),
			se::collision::addVertex(meshData, { -0.866025209f, -0.500000298f, -1.0f }),
			se::collision::addVertex(meshData, { -0.866025209f, -0.500000298f, 1.0f }),
			se::collision::addVertex(meshData, { -1.0f, -0.000000464f, -1.0f }),
			se::collision::addVertex(meshData, { -1.0f, -0.000000464f, 1.0f }),
			se::collision::addVertex(meshData, { -0.866025686f, 0.499999493f, -1.0f }),
			se::collision::addVertex(meshData, { -0.866025686f, 0.499999493f, 1.0f }),
			se::collision::addVertex(meshData, { -0.500000596f, 0.866025090f, -1.0f }),
			se::collision::addVertex(meshData, { -0.500000596f, 0.866025090f, 1.0f })
		};
		std::array<std::array<int, 4>, 48> faceIndices = {{
			{{ vertexIndices[0], vertexIndices[2], vertexIndices[3], vertexIndices[1] }},
			{{ vertexIndices[2], vertexIndices[4], vertexIndices[5], vertexIndices[3] }},
			{{ vertexIndices[4], vertexIndices[6], vertexIndices[7], vertexIndices[5] }},
			{{ vertexIndices[6], vertexIndices[8], vertexIndices[9], vertexIndices[7] }},
			{{ vertexIndices[8], vertexIndices[10], vertexIndices[11], vertexIndices[9] }},
			{{ vertexIndices[10], vertexIndices[12], vertexIndices[13], vertexIndices[11] }},
			{{ vertexIndices[12], vertexIndices[14], vertexIndices[15], vertexIndices[13] }},
			{{ vertexIndices[14], vertexIndices[16], vertexIndices[17], vertexIndices[15] }},
			{{ vertexIndices[16], vertexIndices[18], vertexIndices[19], vertexIndices[17] }},
			{{ vertexIndices[18], vertexIndices[20], vertexIndices[21], vertexIndices[19] }},
			{{ vertexIndices[20], vertexIndices[22], vertexIndices[23], vertexIndices[21] }},
			{{ vertexIndices[22], vertexIndices[0], vertexIndices[1], vertexIndices[23] }},
			{{ vertexIndices[24], vertexIndices[25], vertexIndices[27], vertexIndices[26] }},
			{{ vertexIndices[26], vertexIndices[27], vertexIndices[29], vertexIndices[28] }},
			{{ vertexIndices[28], vertexIndices[29], vertexIndices[31], vertexIndices[30] }},
			{{ vertexIndices[30], vertexIndices[31], vertexIndices[33], vertexIndices[32] }},
			{{ vertexIndices[32], vertexIndices[33], vertexIndices[35], vertexIndices[34] }},
			{{ vertexIndices[34], vertexIndices[35], vertexIndices[37], vertexIndices[36] }},
			{{ vertexIndices[36], vertexIndices[37], vertexIndices[39], vertexIndices[38] }},
			{{ vertexIndices[38], vertexIndices[39], vertexIndices[41], vertexIndices[40] }},
			{{ vertexIndices[40], vertexIndices[41], vertexIndices[43], vertexIndices[42] }},
			{{ vertexIndices[42], vertexIndices[43], vertexIndices[45], vertexIndices[44] }},
			{{ vertexIndices[44], vertexIndices[45], vertexIndices[47], vertexIndices[46] }},
			{{ vertexIndices[46], vertexIndices[47], vertexIndices[25], vertexIndices[24] }},
			{{ vertexIndices[13], vertexIndices[15], vertexIndices[39], vertexIndices[37] }},
			{{ vertexIndices[37], vertexIndices[35], vertexIndices[11], vertexIndices[13] }},
			{{ vertexIndices[35], vertexIndices[33], vertexIndices[9], vertexIndices[11] }},
			{{ vertexIndices[33], vertexIndices[31], vertexIndices[7], vertexIndices[9] }},
			{{ vertexIndices[31], vertexIndices[29], vertexIndices[5], vertexIndices[7] }},
			{{ vertexIndices[29], vertexIndices[27], vertexIndices[3], vertexIndices[5] }},
			{{ vertexIndices[27], vertexIndices[25], vertexIndices[1], vertexIndices[3] }},
			{{ vertexIndices[25], vertexIndices[47], vertexIndices[23], vertexIndices[1] }},
			{{ vertexIndices[47], vertexIndices[45], vertexIndices[21], vertexIndices[23] }},
			{{ vertexIndices[45], vertexIndices[43], vertexIndices[19], vertexIndices[21] }},
			{{ vertexIndices[43], vertexIndices[41], vertexIndices[17], vertexIndices[19] }},
			{{ vertexIndices[41], vertexIndices[39], vertexIndices[15], vertexIndices[17] }},
			{{ vertexIndices[20], vertexIndices[18], vertexIndices[42], vertexIndices[44] }},
			{{ vertexIndices[16], vertexIndices[40], vertexIndices[42], vertexIndices[18] }},
			{{ vertexIndices[14], vertexIndices[38], vertexIndices[40], vertexIndices[16] }},
			{{ vertexIndices[12], vertexIndices[36], vertexIndices[38], vertexIndices[14] }},
			{{ vertexIndices[10], vertexIndices[34], vertexIndices[36], vertexIndices[12] }},
			{{ vertexIndices[8], vertexIndices[32], vertexIndices[34], vertexIndices[10] }},
			{{ vertexIndices[6], vertexIndices[30], vertexIndices[32], vertexIndices[8] }},
			{{ vertexIndices[4], vertexIndices[28], vertexIndices[30], vertexIndices[6] }},
			{{ vertexIndices[2], vertexIndices[26], vertexIndices[28], vertexIndices[4] }},
			{{ vertexIndices[0], vertexIndices[24], vertexIndices[26], vertexIndices[2] }},
			{{ vertexIndices[22], vertexIndices[46], vertexIndices[24], vertexIndices[0] }},
			{{ vertexIndices[20], vertexIndices[44], vertexIndices[46], vertexIndices[22] }}
		}};

		for (const auto& face : faceIndices) {
			addFace(meshData, face.begin(), face.end());
		}

		return meshData;
	}

// Public functions
	Level::Level(GameData& gameData) :
		IGameScreen(gameData), mPlayerEntity(se::app::kNullEntity), mPlayerController(nullptr),
		mLogoTexture(nullptr), mReticleTexture(nullptr), mPickText(nullptr)
	{
		mGameData.eventManager->subscribe(this, se::app::Topic::Key);

		/*********************************************************************
		 * GRAPHICS DATA
		 *********************************************************************/
		se::app::TerrainLoader terrainLoader(*mGameData.entityDatabase, *mGameData.graphicsEngine, *mGameData.cameraSystem, *mGameData.repository);
		se::collision::QuickHull qh(0.0001f);
		se::collision::HACD hacd(0.002f, 0.0002f);

		se::app::Image<unsigned char> heightMap1, splatMap1, logo1, reticle1;
		se::app::Image<float> environment1;
		std::shared_ptr<se::graphics::Mesh> cubeMesh = nullptr, planeMesh = nullptr;
		std::shared_ptr<se::graphics::Texture> logoTexture, reticleTexture, chessTexture, splatmapTexture,
			skyTexture, environmentTexture, prefilterTexture, brdfTexture;
		std::shared_ptr<se::graphics::Font> arial = nullptr;
		se::app::Scenes loadedScenes;

		try {
			// Readers
			AudioFile<float> audioFile;
			auto sceneReader = se::app::SceneReader::createSceneReader(se::app::SceneFileType::GLTF);

			// Meshes
			se::app::RawMesh cubeRawMesh = se::app::MeshLoader::createBoxMesh("Cube", glm::vec3(1.0f));
			cubeRawMesh.normals = se::app::MeshLoader::calculateNormals(cubeRawMesh.positions, cubeRawMesh.faceIndices);
			cubeRawMesh.tangents = se::app::MeshLoader::calculateTangents(cubeRawMesh.positions, cubeRawMesh.texCoords, cubeRawMesh.faceIndices);
			cubeMesh = std::make_shared<se::graphics::Mesh>(se::app::MeshLoader::createGraphicsMesh(cubeRawMesh));

			se::app::RawMesh planeRawMesh("Plane");
			planeRawMesh.positions = { {-0.5f,-0.5f, 0.0f}, { 0.5f,-0.5f, 0.0f}, {-0.5f, 0.5f, 0.0f}, { 0.5f, 0.5f, 0.0f} };
			planeRawMesh.texCoords = { {0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f} };
			planeRawMesh.faceIndices = { 0, 1, 2, 1, 3, 2, };
			planeRawMesh.normals = se::app::MeshLoader::calculateNormals(planeRawMesh.positions, planeRawMesh.faceIndices);
			planeRawMesh.tangents = se::app::MeshLoader::calculateTangents(planeRawMesh.positions, planeRawMesh.texCoords, planeRawMesh.faceIndices);
			planeMesh = std::make_shared<se::graphics::Mesh>(se::app::MeshLoader::createGraphicsMesh(planeRawMesh));

			// Programs
			auto programSky = se::app::TechniqueLoader::createProgram("res/shaders/vertex3D.glsl", nullptr, "res/shaders/fragmentCubeMap.glsl");
			if (!programSky) {
				throw std::runtime_error("programSky not found");
			}
			mGameData.repository->add(std::string("programSky"), std::move(programSky));

			auto programGBufMaterial = se::app::TechniqueLoader::createProgram("res/shaders/vertexNormalMap.glsl", nullptr, "res/shaders/fragmentGBufMaterial.glsl");
			if (!programGBufMaterial) {
				throw std::runtime_error("programGBufMaterial not found");
			}
			mGameData.repository->add(std::string("programGBufMaterial"), std::move(programGBufMaterial));

			auto programGBufMaterialSkinning = se::app::TechniqueLoader::createProgram("res/shaders/vertexNormalMapSkinning.glsl", nullptr, "res/shaders/fragmentGBufMaterial.glsl");
			if (!programGBufMaterialSkinning) {
				throw std::runtime_error("programGBufMaterialSkinning not found");
			}
			mGameData.repository->add(std::string("programGBufMaterialSkinning"), std::move(programGBufMaterialSkinning));

			auto programGBufSplatmap = se::app::TechniqueLoader::createProgram("res/shaders/vertexTerrain.glsl", "res/shaders/geometryTerrain.glsl", "res/shaders/fragmentGBufSplatmap.glsl");
			if (!programGBufSplatmap) {
				throw std::runtime_error("programGBufSplatmap not found");
			}
			mGameData.repository->add(std::string("programGBufSplatmap"), std::move(programGBufSplatmap));

			// Fonts
			arial = mGameData.repository->find<std::string, se::graphics::Font>("arial");
			if (!arial) {
				throw std::runtime_error("Arial font not found");
			}

			// GLTF scenes
			se::app::Result result = sceneReader->load("res/meshes/test.gltf", loadedScenes);
			if (!result) {
				throw std::runtime_error(result.description());
			}

			// Images
			result = se::app::ImageReader::read("res/images/logo.png", logo1);
			if (!result) {
				throw std::runtime_error(result.description());
			}

			result = se::app::ImageReader::read("res/images/terrain.png", heightMap1, 1);
			if (!result) {
				throw std::runtime_error(result.description());
			}

			result = se::app::ImageReader::read("res/images/splatmap.png", splatMap1);
			if (!result) {
				throw std::runtime_error(result.description());
			}

			result = se::app::ImageReader::read("res/images/reticle.png", reticle1);
			if (!result) {
				throw std::runtime_error(result.description());
			}

			result = se::app::ImageReader::readHDR("res/images/piazza_san_marco_2k.hdr", environment1);
			if (!result) {
				throw std::runtime_error(result.description());
			}

			// Textures
			logoTexture = std::make_shared<se::graphics::Texture>(se::graphics::TextureTarget::Texture2D);
			logoTexture->setImage(
				logo1.pixels.get(), se::graphics::TypeId::UnsignedByte, se::graphics::ColorFormat::RGBA, se::graphics::ColorFormat::RGBA,
				logo1.width, logo1.height
			);

			reticleTexture = std::make_shared<se::graphics::Texture>(se::graphics::TextureTarget::Texture2D);
			reticleTexture->setImage(
				reticle1.pixels.get(), se::graphics::TypeId::UnsignedByte, se::graphics::ColorFormat::RGBA, se::graphics::ColorFormat::RGBA,
				reticle1.width, reticle1.height
			);

			float pixels[] = {
				0.0f, 0.0f, 0.0f,	1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f,	0.0f, 0.0f, 0.0f
			};
			chessTexture = std::make_shared<se::graphics::Texture>(se::graphics::TextureTarget::Texture2D);
			chessTexture->setImage(pixels, se::graphics::TypeId::Float, se::graphics::ColorFormat::RGB, se::graphics::ColorFormat::RGB, 2, 2);

			splatmapTexture = std::make_shared<se::graphics::Texture>(se::graphics::TextureTarget::Texture2D);
			splatmapTexture->setWrapping(se::graphics::TextureWrap::ClampToEdge, se::graphics::TextureWrap::ClampToEdge)
				.setImage(
					splatMap1.pixels.get(), se::graphics::TypeId::UnsignedByte, se::graphics::ColorFormat::RGBA,
					se::graphics::ColorFormat::RGBA, splatMap1.width, splatMap1.height
				);

			auto environmentEquiTexture = std::make_shared<se::graphics::Texture>(se::graphics::TextureTarget::Texture2D);
			environmentEquiTexture->setWrapping(se::graphics::TextureWrap::ClampToEdge, se::graphics::TextureWrap::ClampToEdge)
				.setFiltering(se::graphics::TextureFilter::Linear, se::graphics::TextureFilter::Linear)
				.setImage(
					environment1.pixels.get(), se::graphics::TypeId::Float, se::graphics::ColorFormat::RGB,
					se::graphics::ColorFormat::RGB, environment1.width, environment1.height
				);
			skyTexture = se::app::TextureUtils::equirectangularToCubeMap(environmentEquiTexture, 512);
			environmentTexture = se::app::TextureUtils::convoluteCubeMap(skyTexture, 32);
			prefilterTexture = se::app::TextureUtils::prefilterCubeMap(skyTexture, 128);
			brdfTexture = se::app::TextureUtils::precomputeBRDF(512);

			// Audio
			if (!audioFile.load("res/audio/bounce.wav")) {
				throw std::runtime_error("Error reading the audio file");
			}

			mBuffers.emplace_back(
				audioFile.samples[0].data(), audioFile.samples[0].size() * sizeof(float),
				se::audio::FormatId::MonoFloat, audioFile.getSampleRate()
			);
		}
		catch (std::exception& e) {
			SOMBRA_ERROR_LOG << "Error: " << e.what();
			return;
		}

		auto gBufferRenderer = static_cast<se::graphics::Renderer*>(mGameData.graphicsEngine->getRenderGraph().getNode("gBufferRenderer"));
		auto forwardRenderer = static_cast<se::graphics::Renderer*>(mGameData.graphicsEngine->getRenderGraph().getNode("forwardRenderer"));
		auto programGBufMaterial = mGameData.repository->find<std::string, se::graphics::Program>("programGBufMaterial");

		// Forces
		mForces.push_back(new se::physics::Gravity({ 0.0f, -9.8f, 0.0f }));
		se::physics::Force* gravity = mForces.back();

		// Renderable2Ds
		auto technique2D = mGameData.repository->find<std::string, se::graphics::Technique>("technique2D");
		mLogoTexture = new se::graphics::RenderableSprite(glm::vec2(1060.0f, 20.0f), glm::vec2(200.0f, 200.0f), glm::vec4(1.0f), logoTexture);
		mLogoTexture->addTechnique(technique2D);
		mLogoTexture->setZIndex(255);
		mGameData.graphicsEngine->addRenderable(mLogoTexture);
		mReticleTexture = new se::graphics::RenderableSprite(glm::vec2(kWidth / 2.0f - 10.0f, kHeight / 2.0f - 10.0f), glm::vec2(20.0f, 20.0f), glm::vec4(1.0f, 1.0f, 1.0f, 0.6f), reticleTexture);
		mReticleTexture->addTechnique(technique2D);
		mReticleTexture->setZIndex(255);
		mGameData.graphicsEngine->addRenderable(mReticleTexture);
		mPickText = new se::graphics::RenderableText({ 0.0f, 700.0f }, glm::vec2(16.0f), arial, { 0.0f, 1.0f, 0.0f, 1.0f });
		mPickText->addTechnique(technique2D);
		mPickText->setZIndex(255);
		mGameData.graphicsEngine->addRenderable(mPickText);

		/*********************************************************************
		 * GAME DATA
		 *********************************************************************/
		mGameData.appRenderer->setIrradianceMap(environmentTexture);
		mGameData.appRenderer->setPrefilterMap(prefilterTexture);
		mGameData.appRenderer->setBRDFMap(brdfTexture);

		// Player
		{
			mPlayerEntity = mGameData.entityDatabase->addEntity();
			mEntities.push_back(mPlayerEntity);

			mGameData.entityDatabase->addComponent(mPlayerEntity, se::app::TagComponent("player"));

			se::app::TransformsComponent transforms;
			transforms.position = glm::vec3(0.0f, 1.0f, 10.0f);
			transforms.orientation = glm::quat(glm::vec3(0.0f, glm::pi<float>(), 0.0f));
			mGameData.entityDatabase->addComponent(mPlayerEntity, std::move(transforms));

			se::physics::RigidBodyConfig config(0.001f);
			config.invertedMass = 1.0f / 40.0f;	// No inertia tensor so the player can't rotate due to collisions
			config.linearDrag = 0.01f;
			config.angularDrag = 0.01f;
			config.frictionCoefficient = 1.16f;
			se::physics::RigidBody rigidBody(config, se::physics::RigidBodyData());
			mGameData.entityDatabase->addComponent(mPlayerEntity, std::move(rigidBody));

			auto collider = std::make_unique<se::collision::BoundingSphere>(0.5f);
			mGameData.entityDatabase->addComponent<se::collision::Collider>(mPlayerEntity, std::move(collider));

			se::app::Camera camera;
			camera.setPerspectiveProjectionMatrix(glm::radians(kFOV), kWidth / static_cast<float>(kHeight), kZNear, kZFar);
			mGameData.entityDatabase->addComponent(mPlayerEntity, std::move(camera));

			se::app::LightSource spotLight(se::app::LightSource::Type::Spot);
			spotLight.name = "spotLight";
			spotLight.intensity = 5.0f;
			spotLight.range = 20.0f;
			spotLight.innerConeAngle = glm::pi<float>() / 12.0f;
			spotLight.outerConeAngle = glm::pi<float>() / 6.0f;
			mGameData.entityDatabase->addComponent(mPlayerEntity, std::move(spotLight));

			mGameData.audioSystem->setListener(mPlayerEntity);
		}

		// Sky
		if (false) {
			auto skyEntity = mGameData.entityDatabase->addEntity();
			mEntities.push_back(skyEntity);

			mGameData.entityDatabase->addComponent(mPlayerEntity, se::app::TagComponent("sky"));

			se::app::TransformsComponent transforms;
			transforms.scale = glm::vec3(kZFar / 2.0f);
			mGameData.entityDatabase->addComponent(mPlayerEntity, std::move(transforms));

			auto programSky = mGameData.repository->find<std::string, se::graphics::Program>("programSky");
			auto passSky = mGameData.cameraSystem->createPass3D(forwardRenderer, programSky);
			skyTexture->setTextureUnit(0);
			passSky->addBindable(skyTexture)
				.addBindable(std::make_shared<se::graphics::UniformVariableValue<int>>("uCubeMap", *programSky, 0))
				.addBindable(std::make_shared<se::graphics::CullingOperation>(false));

			auto techniqueSky = std::make_unique<se::graphics::Technique>();
			techniqueSky->addPass(passSky);

			se::graphics::RenderableMesh renderableMesh(cubeMesh);
			renderableMesh.addTechnique(std::move(techniqueSky));

			mGameData.entityDatabase->addComponent(skyEntity, std::move(renderableMesh));
		}

		// Terrain
		{
			se::app::SplatmapMaterial terrainMaterial;
			terrainMaterial.name = "terrainMaterial";
			terrainMaterial.splatmapTexture = std::move(splatmapTexture);
			terrainMaterial.materials.push_back({ se::app::PBRMetallicRoughness{ { 0.5f, 0.25f, 0.1f, 1.0f }, {}, 0.2f, 0.5f, {} }, {}, 1.0f });
			terrainMaterial.materials.push_back({ se::app::PBRMetallicRoughness{ { 0.1f, 0.75f, 0.25f, 1.0f }, {}, 0.2f, 0.5f, {} }, {}, 1.0f });
			terrainMaterial.materials.push_back({ se::app::PBRMetallicRoughness{ { 0.1f, 0.25f, 0.75f, 1.0f }, {}, 0.2f, 0.5f, {} }, {}, 1.0f });

			std::vector<float> lodDistances{ 2000.0f, 1000.0f, 500.0f, 250.0f, 125.0f, 75.0f, 40.0f, 20.0f, 10.0f, 0.0f };
			mEntities.push_back( terrainLoader.createTerrain("terrain", 500.0f, 10.0f, heightMap1, lodDistances, terrainMaterial, "programGBufSplatmap") );
		}

		// Plane
		{
			auto plane = mGameData.entityDatabase->addEntity();
			mEntities.push_back(plane);

			mGameData.entityDatabase->addComponent(plane, se::app::TagComponent("plane"));

			se::app::TransformsComponent transforms;
			transforms.position = glm::vec3(-15.0f, 1.0f, -5.0f);
			mGameData.entityDatabase->addComponent(plane, std::move(transforms));

			auto passPlane = mGameData.cameraSystem->createPass3D(gBufferRenderer, programGBufMaterial);
			se::app::TechniqueLoader::addMaterialBindables(
				passPlane,
				se::app::Material{
					"plane_material",
					se::app::PBRMetallicRoughness{ glm::vec4(1.0f), {}, 0.2f, 0.5f, {} },
					{}, 1.0f, {}, 1.0f, chessTexture, glm::vec3(1.0f), se::graphics::AlphaMode::Opaque, 0.5f, true
				},
				programGBufMaterial
			);

			auto techniquePlane = std::make_unique<se::graphics::Technique>();
			techniquePlane->addPass(passPlane);

			se::graphics::RenderableMesh renderableMesh(planeMesh);
			renderableMesh.addTechnique(std::move(techniquePlane));

			mGameData.entityDatabase->addComponent(plane, std::move(renderableMesh));
		}

		// Fixed cubes
		glm::vec3 cubePositions[5] = {
			{ 2.0f, 5.0f, -10.0f },
			{ 0.0f, 7.0f, -10.0f },
			{ 0.0f, 5.0f, -8.0f },
			{ 0.0f, 5.0f, -10.0f },
			{ 10.0f, 5.0f, -10.0f }
		};
		glm::vec4 colors[5] = { { 1.0f, 0.2f, 0.2f, 1.0f }, { 0.2f, 1.0f, 0.2f, 1.0f }, { 0.2f, 0.2f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.2f, 0.2f, 0.1f, 1.0f } };
		se::app::Entity e1, e2;
		for (std::size_t i = 0; i < 5; ++i) {
			auto cube = mGameData.entityDatabase->addEntity();
			mEntities.push_back(cube);

			mGameData.entityDatabase->addComponent(cube, se::app::TagComponent("non-random-cube-" + std::to_string(i)));

			se::app::TransformsComponent transforms;
			transforms.position = cubePositions[i];

			se::physics::RigidBodyConfig config(20.0f, 2.0f / 5.0f * 10.0f * glm::pow(2.0f, 2.0f) * glm::mat3(1.0f), 0.001f);
			config.linearDrag = 0.95f;
			config.angularDrag = 0.95f;
			config.frictionCoefficient = 0.5f;
			se::physics::RigidBody rigidBody(config, se::physics::RigidBodyData());
			if (i == 1) {
				e1 = cube;
			}
			if (i == 2) {
				se::audio::Source source1;
				source1.bind(mBuffers.back());
				source1.setLooping(true);
				source1.play();
				mGameData.entityDatabase->addComponent(cube, std::move(source1));
			}
			if (i == 3) {
				rigidBody.getData().angularVelocity = glm::vec3(0.0f, 10.0f, 0.0f);
				e2 = cube;
			}
			if (i == 4) {
				transforms.velocity += glm::vec3(-1, 0, 0);
			}

			mGameData.entityDatabase->addComponent(cube, std::move(transforms));
			mGameData.entityDatabase->addComponent(cube, std::move(rigidBody));

			auto collider = std::make_unique<se::collision::BoundingBox>(glm::vec3(1.0f, 1.0f, 1.0f));
			mGameData.entityDatabase->addComponent<se::collision::Collider>(cube, std::move(collider));

			auto passCube = mGameData.cameraSystem->createPass3D(gBufferRenderer, programGBufMaterial);
			se::app::TechniqueLoader::addMaterialBindables(
				passCube,
				se::app::Material{
					"tmp_material",
					se::app::PBRMetallicRoughness{ colors[i], {}, 0.9f, 0.1f, {} },
					{}, 1.0f, {}, 1.0f, {}, glm::vec3(0.0f), se::graphics::AlphaMode::Opaque, 0.5f, false
				},
				programGBufMaterial
			);

			auto techniqueCube = std::make_unique<se::graphics::Technique>();
			techniqueCube->addPass(passCube);

			se::graphics::RenderableMesh renderableMesh(cubeMesh);
			renderableMesh.addTechnique(std::move(techniqueCube));

			mGameData.entityDatabase->addComponent(cube, std::move(renderableMesh));
		}

		auto [rb1] = mGameData.entityDatabase->getComponents<se::physics::RigidBody>(e1);
		auto [rb2] = mGameData.entityDatabase->getComponents<se::physics::RigidBody>(e2);
		mConstraints.push_back(new se::physics::DistanceConstraint({ rb1, rb2 }));
		mGameData.physicsEngine->getConstraintManager().addConstraint(mConstraints.back());

		auto passRed = mGameData.cameraSystem->createPass3D(gBufferRenderer, programGBufMaterial);
		se::app::TechniqueLoader::addMaterialBindables(
			passRed,
			se::app::Material{
				"tmp_material",
				se::app::PBRMetallicRoughness{ { 1.0f, 0.0f, 0.0f, 1.0f }, {}, 0.2f, 0.5f, {} },
				{}, 1.0f, {}, 1.0f, {}, glm::vec3(0.0f), se::graphics::AlphaMode::Opaque, 0.5f, false
			},
			programGBufMaterial
		);

		auto techniqueRed = std::make_shared<se::graphics::Technique>();
		techniqueRed->addPass(passRed);

		{
			auto nonMovableCube = mGameData.entityDatabase->addEntity();
			mEntities.push_back(nonMovableCube);

			mGameData.entityDatabase->addComponent(nonMovableCube, se::app::TagComponent("non-movable-cube"));

			se::app::TransformsComponent transforms;
			transforms.position = glm::vec3(-50.0f, 0.0f, -40.0f);
			transforms.scale = glm::vec3(10.0f, 1.0f, 10.0f);
			mGameData.entityDatabase->addComponent(nonMovableCube, std::move(transforms));

			se::physics::RigidBodyConfig config(0.001f);
			config.frictionCoefficient = 0.75f;
			se::physics::RigidBody rigidBody(config, se::physics::RigidBodyData());
			mGameData.entityDatabase->addComponent(nonMovableCube, std::move(rigidBody));

			auto collider = std::make_unique<se::collision::BoundingBox>(glm::vec3(1.0f));
			mGameData.entityDatabase->addComponent<se::collision::Collider>(nonMovableCube, std::move(collider));

			se::graphics::RenderableMesh renderableMesh(cubeMesh);
			renderableMesh.addTechnique(techniqueRed);
			mGameData.entityDatabase->addComponent(nonMovableCube, std::move(renderableMesh));
		}

		{
			auto gravityCube = mGameData.entityDatabase->addEntity();
			mEntities.push_back(gravityCube);

			mGameData.entityDatabase->addComponent(gravityCube, se::app::TagComponent("gravity-cube"));

			se::app::TransformsComponent transforms;
			transforms.position = glm::vec3(-50.0f, 2.0f, -40.0f);
			mGameData.entityDatabase->addComponent(gravityCube, std::move(transforms));

			se::physics::RigidBodyConfig config(20.0f, 2.0f / 5.0f * 10.0f * glm::pow(2.0f, 2.0f) * glm::mat3(1.0f), 0.001f);
			config.linearDrag = 0.95f;
			config.angularDrag = 0.95f;
			config.frictionCoefficient = 0.65f;
			se::physics::RigidBody rigidBody(config, se::physics::RigidBodyData());
			mGameData.entityDatabase->addComponent(gravityCube, std::move(rigidBody));
			auto [rb] = mGameData.entityDatabase->getComponents<se::physics::RigidBody>(gravityCube);
			mGameData.physicsEngine->getForceManager().addRBForce(rb, gravity);

			auto collider3 = std::make_unique<se::collision::BoundingBox>(glm::vec3(1.0f));
			mGameData.entityDatabase->addComponent<se::collision::Collider>(gravityCube, std::move(collider3));

			se::graphics::RenderableMesh renderableMesh(cubeMesh);
			renderableMesh.addTechnique(techniqueRed);
			mGameData.entityDatabase->addComponent(gravityCube, std::move(renderableMesh));
		}

		// HACD Tube
		se::collision::HalfEdgeMesh tube = createTestTube1();
		glm::vec3 tubeCentroid = se::collision::calculateCentroid(tube);
		hacd.calculate(tube);
		for (const auto& [heMesh, normals] : hacd.getMeshes()) {
			glm::vec3 sliceCentroid = se::collision::calculateCentroid(heMesh);
			glm::vec3 displacement = sliceCentroid - tubeCentroid;
			if (glm::length(displacement) > 0.0f) {
				displacement = glm::normalize(displacement);
			}
			displacement *= 0.1f;

			auto tubeSlice = mGameData.entityDatabase->addEntity();
			mEntities.push_back(tubeSlice);

			mGameData.entityDatabase->addComponent(tubeSlice, se::app::TagComponent("tubeSlice"));

			se::app::TransformsComponent transforms;
			transforms.orientation = glm::normalize(glm::quat(-1, glm::vec3(1.0f, 0.0f, 0.0f)));
			transforms.position = glm::vec3(0.0f, 2.0f, 75.0f) + displacement;
			mGameData.entityDatabase->addComponent(tubeSlice, std::move(transforms));

			auto passSlice = mGameData.cameraSystem->createPass3D(gBufferRenderer, programGBufMaterial);
			se::app::TechniqueLoader::addMaterialBindables(
				passSlice,
				se::app::Material{
					"tmp_material",
					se::app::PBRMetallicRoughness{
						glm::vec4(glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f), 1.0f),
						{}, 0.2f, 0.5f, {}
					},
					{}, 1.0f, {}, 1.0f, {}, glm::vec3(0.0f), se::graphics::AlphaMode::Opaque, 0.5f, false
				},
				programGBufMaterial
			);

			auto techniqueSlice = std::make_shared<se::graphics::Technique>();
			techniqueSlice->addPass(passSlice);

			auto tmpRawMesh = se::app::MeshLoader::createRawMesh(heMesh, normals).first;
			auto tmpGraphicsMesh = std::make_shared<se::graphics::Mesh>(se::app::MeshLoader::createGraphicsMesh(tmpRawMesh));
			se::graphics::RenderableMesh renderableMesh(tmpGraphicsMesh);
			renderableMesh.addTechnique(techniqueSlice);

			mGameData.entityDatabase->addComponent(tubeSlice, std::move(renderableMesh));
		}

		// Random cubes
		{
			auto passRandom = mGameData.cameraSystem->createPass3D(gBufferRenderer, programGBufMaterial);
			se::app::TechniqueLoader::addMaterialBindables(
				passRandom,
				se::app::Material{
					"random",
					se::app::PBRMetallicRoughness{ { 0.0f, 0.0f, 1.0f, 1.0f }, {}, 0.2f, 0.5f, {} },
					{}, 1.0f, {}, 1.0f, {}, glm::vec3(0.0f), se::graphics::AlphaMode::Opaque, 0.5f, false
				},
				programGBufMaterial
			);

			auto techniqueRandom = std::make_shared<se::graphics::Technique>();
			techniqueRandom->addPass(passRandom);

			for (std::size_t i = 0; i < kNumCubes; ++i) {
				auto cube = mGameData.entityDatabase->addEntity();
				mEntities.push_back(cube);

				mGameData.entityDatabase->addComponent(cube, se::app::TagComponent("random-cube-" + std::to_string(i)));

				se::app::TransformsComponent transforms;
				transforms.position = glm::ballRand(50.0f);
				mGameData.entityDatabase->addComponent(cube, std::move(transforms));

				se::physics::RigidBodyConfig config(10.0f, 2.0f / 5.0f * 10.0f * glm::pow(2.0f, 2.0f) * glm::mat3(1.0f), 0.001f);
				config.linearDrag = 0.9f;
				config.angularDrag = 0.9f;
				config.frictionCoefficient = 0.5f;
				se::physics::RigidBody rigidBody(config, se::physics::RigidBodyData());
				mGameData.entityDatabase->addComponent(cube, std::move(rigidBody));
				auto [rb] = mGameData.entityDatabase->getComponents<se::physics::RigidBody>(cube);
				mGameData.physicsEngine->getForceManager().addRBForce(rb, gravity);

				auto collider = std::make_unique<se::collision::BoundingBox>(glm::vec3(1.0f, 1.0f, 1.0f));
				mGameData.entityDatabase->addComponent<se::collision::Collider>(cube, std::move(collider));

				se::graphics::RenderableMesh renderableMesh(cubeMesh);
				renderableMesh.addTechnique(techniqueRandom);
				mGameData.entityDatabase->addComponent(cube, std::move(renderableMesh));
			}
		}

		// GLTF Scene
		/*{ FIXME:
			auto sceneEntity = std::make_unique<se::app::Entity>("Scene");
			mGameData.animationManager->addEntity(sceneEntity.get(), std::move(loadedScenes.scenes[0]->rootNode));
			mEntities.push_back( std::move(sceneEntity) );

			std::vector<std::shared_ptr<se::app::Skin>> sharedSkins;
			std::move(loadedScenes.skins.begin(), loadedScenes.skins.end(), std::back_inserter(sharedSkins));

			std::vector<std::shared_ptr<se::graphics::Technique>> techniques;
			for (auto& material : loadedScenes.materials) {
				auto pass3D = mGameData.graphicsManager->createPass3D(gBufferRenderer, programGBufMaterial, true);
				se::app::TechniqueLoader::addMaterialBindables(pass3D, *material, programGBufMaterial);
				techniques.emplace_back(std::make_shared<se::graphics::Technique>())->addPass(pass3D);
			}

			for (auto& e : loadedScenes.scenes[0]->entities) {
				if (e.animationNode) {
					auto entity = std::make_unique<se::app::Entity>(e.animationNode->getData().name);
					mGameData.animationManager->addEntity(entity.get(), e.animationNode);

					if (e.hasLightSource) {
						mGameData.graphicsManager->addLightEntity(entity.get(), std::move(loadedScenes.lightSources[e.lightSourceIndex]));
					}
					if (e.hasPrimitives) {
						if (e.hasSkin) {
							for (auto [iMesh, iMaterial] : loadedScenes.primitives[e.primitivesIndex]) {
								auto renderableMesh = std::make_unique<se::graphics::RenderableMesh>(std::move(loadedScenes.meshes[iMesh]));
								renderableMesh->addTechnique(techniques[iMaterial]);
								mGameData.graphicsManager->addMeshEntity(entity.get(), std::move(renderableMesh), sharedSkins[e.skinIndex]);
							}
						}
						else {
							for (auto [iMesh, iMaterial] : loadedScenes.primitives[e.primitivesIndex]) {
								auto renderableMesh = std::make_unique<se::graphics::RenderableMesh>(std::move(loadedScenes.meshes[iMesh]));
								renderableMesh->addTechnique(techniques[iMaterial]);
								mGameData.graphicsManager->addMeshEntity(entity.get(), std::move(renderableMesh));
							}
						}
					}

					mEntities.push_back( std::move(entity) );
				}
			}
		}*/

		setHandleInput(true);
	}


	Level::~Level()
	{
		setHandleInput(false);

		for (se::physics::Force* force : mForces) {
			mGameData.physicsEngine->getForceManager().removeForce(force);
			delete force;
		}

		for (se::physics::Constraint* constraint : mConstraints) {
			mGameData.physicsEngine->getConstraintManager().removeConstraint(constraint);
			delete constraint;
		}

		for (se::animation::IAnimator* animator : mAnimators) {
			mGameData.animationEngine->removeAnimator(animator);
			delete animator;
		}

		for (auto entity : mEntities) {
			mGameData.entityDatabase->removeEntity(entity);
		}

		mGameData.graphicsEngine->removeRenderable(mLogoTexture);
		delete mLogoTexture;
		mGameData.graphicsEngine->removeRenderable(mReticleTexture);
		delete mReticleTexture;
		mGameData.graphicsEngine->removeRenderable(mPickText);
		delete mPickText;

		mGameData.eventManager->unsubscribe(this, se::app::Topic::Key);
	}


	void Level::update(float deltaTime)
	{
		if (mPlayerController) {
			mPlayerController->update(deltaTime);
		}
	}


	void Level::notify(const se::app::IEvent& event)
	{
		// Check if we shouldn't handle the input
		if (mPlayerController) {
			tryCall(&Level::onKeyEvent, event);
		}
	}


	void Level::setHandleInput(bool handle)
	{
		if (handle && !mPlayerController) {
			mPlayerController = new PlayerController(mGameData, mPlayerEntity, *mPickText);
			mPlayerController->resetMousePosition();
			mGameData.windowSystem->setCursorVisibility(false);
		}

		if (!handle && mPlayerController) {
			mPlayerController->resetMousePosition();
			delete mPlayerController;
			mPlayerController = nullptr;

			mGameData.windowSystem->setCursorVisibility(true);
		}
	}

// Private functions
	void Level::onKeyEvent(const se::app::KeyEvent& event)
	{
		if ((event.getKeyCode() == SE_KEY_ESCAPE) && (event.getState() != se::app::KeyEvent::State::Released)) {
			mGameData.stateMachine->submitEvent(static_cast<se::utils::StateMachine::Event>(GameEvent::AddGameMenu));
		}
	}

}
