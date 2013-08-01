#include"ODEGLLIB.h"
void ODE_TranslateRotate_2_OpenGL(GLfloat *local_matrix, dGeomID geom)
{
	if (!local_matrix) return;
	if (geom==0) return;
	const dReal *pos=dGeomGetPosition(geom);
	const dReal *R=dGeomGetRotation(geom);
	local_matrix[0] = R[0];
	local_matrix[1] = R[4];
	local_matrix[2] = R[8];
	local_matrix[3] = 0;
	local_matrix[4] = R[1];
	local_matrix[5] = R[5];
	local_matrix[6] = R[9];
	local_matrix[7] = 0;
	local_matrix[8] = R[2];
	local_matrix[9] = R[6];
	local_matrix[10] = R[10];
	local_matrix[11] = 0;
	local_matrix[12] = pos[0];
	local_matrix[13] = pos[1];
	local_matrix[14] = pos[2];
	local_matrix[15] = 1;
}