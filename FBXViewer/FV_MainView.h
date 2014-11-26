#pragma once

namespace FbxViewer
{
	class SceneContext;

	void FV_MainView_Enter();

	void FV_MainView_LoadFBX(const char* szFileName);

	SceneContext* FV_GetSceneContext(void);
}
