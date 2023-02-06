/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Class for openGL calls.
 */


#pragma once
#include <chrono>
#include <string>
#include <vector>
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_vec.hpp>


class SkyBox;
class Texture;
class SimpleBox;
class PhysicsSimulation;
class Floor;
class Buffer;
class Line;
class Shader;
struct aiScene;
class Camera;
class GLFWwindow;
class AnimationModel;
class Object;

const static std::string bobLampPath = "../Models/boblampclean.md5mesh";
const static std::string hellKnightPath = "../Models/hellknight/hellknight.md5mesh";
const static std::string mutantPath = "../Models/Mutant.dae";
const static std::string goblinPath = "../Models/Goblin.dae";
const static std::string guardPath = "../Models/Guard.dae";
const static std::string ch24Path = "../Models/Ch24.dae";
const static std::string multipleAnimationExample = "../Models/Prop2.fbx";
const static std::string path = "../Models/Slow Run.dae";

class Graphic
{
public:
	Graphic();
	~Graphic();

	void Populate();
	void Draw(float dt);
	void DrawLine(glm::mat4 projViewMat_);
	void ProcessInput();
	void InitLineBuffer();
	float DistanceTimeFunction(float t) const;
	void Reset();
	float t1 = 0.2f;
	float t2 = 0.8f;
	float t3 = 1.f;
	Texture* boxTexture;
	GLFWwindow* window;
	Camera* cam;
	AnimationModel* mutant;
	AnimationModel* goblin;
	AnimationModel* ch24;
	AnimationModel* guard;
	AnimationModel* multipleAni;
	SkyBox* skybox;

	std::vector<glm::mat4> totalTransform;
	std::vector<int> offsets;
	PhysicsSimulation* physicsSimulation;
	void ReInitSimulation();

	float deltaTime, lastFrame;
	bool camLock = true;

	int transformsOffset;
	unsigned animationIndex;
	bool showOthers;
	Object* obj;
	Line* line;

	unsigned lineVao;
	Buffer* linePosBuffer;
	std::vector<glm::vec3> lineCoords;
	SimpleBox* simpleBox;

	SimpleBox* frontRight;
	SimpleBox* backRight;
	SimpleBox* frontLeft;
	SimpleBox* backLeft;

private:
	Shader* shader;
	Shader* lineShader;
	Shader* floorShader;
	Shader* dotsShader;
	Floor* floor;
	const int windowWidth;
	const int windowHeight;

	float k1 = 0.3f, k2 = 1.f - k1;

	float PI = 3.14159265359f;

	float v0 = .1f;
	
	std::chrono::system_clock::time_point startTime;
};
