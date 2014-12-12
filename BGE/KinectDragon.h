#pragma once
#include "GameComponent.h"
#include "PhysicsController.h"
#include "NuiApi.h"
#include <string>
#include <map>
#include "SkeletonMapper.h"
#include "DragonSpawner.h"


using namespace std;

namespace BGE
{
	void CALLBACK StatusProc(HRESULT hrStatus, const OLECHAR* instanceName, const OLECHAR* uniqueDeviceName, void* pUserData);


	class KinectDragon :
		public GameComponent
	{
	private:
		bool m_bSeatedMode;
		INuiSensor* m_pNuiSensor;
		HANDLE m_hNextSkeletonEvent;
		void SetStatusMessage(std::string message);
		void UpdateSkeleton(const NUI_SKELETON_DATA & skeleton);
		void SkeletonFrameReady(NUI_SKELETON_FRAME* skeletonFrame);
		void UpdateBone(const NUI_SKELETON_DATA & skeleton, NUI_SKELETON_POSITION_INDEX jointFrom, NUI_SKELETON_POSITION_INDEX jointTo);
		void UpdateDragon(const NUI_SKELETON_DATA & skeleton, NUI_SKELETON_POSITION_INDEX joint);
		void TrackHand(const NUI_SKELETON_DATA & skeleton, NUI_SKELETON_POSITION_INDEX HandPos, NUI_SKELETON_POSITION_INDEX ShoulderCentre, int HandID);
		void UpdateKnob(string tag, glm::vec3 pos);

		map<string, std::shared_ptr<PhysicsController>> boneComponents;
		bool connected;
		bool tracked;
		float footHeight;
		float scale;
		DragonSpawner * Dragons;
		bool FlyUp;
		bool WingUp;
		bool WingDown;

	public:
		KinectDragon(DragonSpawner *dragons);
		~KinectDragon(void);
		void Update();
		bool Initialise();
		HRESULT CreateFirstConnected();
		bool headCamera;
		friend void CALLBACK StatusProc(HRESULT hrStatus, const OLECHAR* instanceName, const OLECHAR* uniqueDeviceName, void* pUserData);

		shared_ptr<SkeletonMapper> mapper;

	};

}