#pragma once
class ViewMatrixManager
{
public:
	ViewMatrixManager();
	~ViewMatrixManager();

	D3DMatrix viewmatrix;
	void worldToScreen(Vector3f pos, Vector3f& screen, float* distance, int* distanceValue);
	void worldBoneToScreen(Vector3f pos, Vector3f& screen);

};

extern ViewMatrixManager* g_pViewMatrixManager;