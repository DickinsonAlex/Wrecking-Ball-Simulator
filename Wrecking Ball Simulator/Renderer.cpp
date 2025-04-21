#include "Renderer.h"
#include "UserData.h"
#include "FontRenderer.h" // Include FontRenderer for text rendering
#include <GL/glut.h> // Include OpenGL Utility Toolkit for window management
#include "Helpful.h"

#include <vector>
#include <string>
#include <iostream> // Include iostream for logging

using namespace std;
using namespace physx;

namespace Renderer
{
	// OpenGL variables
	PxVec3 defaultColor = PxVec3(0.f, 0.f, 0.f);
	PxVec3 backgroundColor = Helpful::RGBtoScalar(198, 244, 246);
	int renderDetail = 10;
	bool renderShadows = true;

	static float gPlaneData[] = {
		-1.f, 0.f, -1.f, 0.f, 1.f, 0.f, -1.f, 0.f, 1.f, 0.f, 1.f, 0.f,
		1.f, 0.f, 1.f, 0.f, 1.f, 0.f, -1.f, 0.f, -1.f, 0.f, 1.f, 0.f,
		1.f, 0.f, 1.f, 0.f, 1.f, 0.f, 1.f, 0.f, -1.f, 0.f, 1.f, 0.f
	};

	void DrawPlane()
	{
		glScalef(10240, 0, 10240);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glVertexPointer(3, GL_FLOAT, 2 * 3 * sizeof(float), gPlaneData);
		glNormalPointer(GL_FLOAT, 2 * 3 * sizeof(float), gPlaneData + 3);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
	}

	void DrawSphere(const PxGeometryHolder& geometry)
	{
		glutSolidSphere(geometry.sphere().radius, renderDetail, renderDetail);
	}

	void DrawBox(const PxGeometryHolder& geometry)
	{
		PxVec3 half_size = geometry.box().halfExtents;
		glScalef(half_size.x, half_size.y, half_size.z);
		glutSolidCube(2.f);
	}

	void DrawCapsule(const PxGeometryHolder& geometry)
	{
		const PxF32 radius = geometry.capsule().radius;
		const PxF32 halfHeight = geometry.capsule().halfHeight;

		//Sphere
		glPushMatrix();
		glTranslatef(halfHeight, 0.f, 0.f);
		glutSolidSphere(radius, renderDetail, renderDetail);
		glPopMatrix();

		//Sphere
		glPushMatrix();
		glTranslatef(-halfHeight, 0.f, 0.f);
		glutSolidSphere(radius, renderDetail, renderDetail);
		glPopMatrix();

		//Cylinder
		glPushMatrix();
		glTranslatef(-halfHeight, 0.f, 0.f);
		glRotatef(90.f, 0.f, 1.f, 0.f);

		GLUquadric* qobj = gluNewQuadric();
		gluQuadricNormals(qobj, GLU_SMOOTH);
		gluCylinder(qobj, radius, radius, halfHeight * 2.f, renderDetail, renderDetail);
		gluDeleteQuadric(qobj);
		glPopMatrix();
	}

	void DrawConvexMesh(const PxGeometryHolder& geometry)
	{
		PxConvexMesh* mesh = geometry.convexMesh().convexMesh;
		PxU32 num_polys = mesh->getNbPolygons();
		const PxVec3* verts = mesh->getVertices();
		const PxU8* indicies = mesh->getIndexBuffer();

		for (PxU32 i = 0; i < num_polys; i++)
		{
			PxHullPolygon face;
			if (mesh->getPolygonData(i, face))
			{
				glBegin(GL_POLYGON);
				glNormal3f(face.mPlane[0], face.mPlane[1], face.mPlane[2]);
				const PxU8* faceIdx = indicies + face.mIndexBase;
				for (PxU32 j = 0; j < face.mNbVerts; j++)
				{
					PxVec3 v = verts[faceIdx[j]];
					glVertex3f(v.x, v.y, v.z);
				}
				glEnd();
			}
		}
	}

	void DrawTriangleMesh(const PxGeometryHolder& geometry)
	{
		PxTriangleMesh* mesh = geometry.triangleMesh().triangleMesh;
		const PxVec3* verts = mesh->getVertices();
		PxU16* trigs = (PxU16*)mesh->getTriangles();
		const PxU32 num_trigs = mesh->getNbTriangles();

		for (PxU32 i = 0; i < num_trigs * 3; i += 3)
		{
			PxVec3 v0 = verts[trigs[i]];
			PxVec3 v1 = verts[trigs[i + 1]];
			PxVec3 v2 = verts[trigs[i + 2]];
			PxVec3 n = (v1 - v0).cross(v2 - v0);
			n.normalize();
			glBegin(GL_POLYGON);
			glNormal3f(n.x, n.y, n.z);
			glVertex3f(v0.x, v0.y, v0.z);
			glVertex3f(v1.x, v1.y, v1.z);
			glVertex3f(v2.x, v2.y, v2.z);
			glEnd();
		}
	}

	void RenderGeometry(const PxGeometryHolder& geometry)
	{
		switch (geometry.getType())
		{
		case PxGeometryType::ePLANE:
			DrawPlane();
			break;
		case PxGeometryType::eSPHERE:
			DrawSphere(geometry);
			break;
		case PxGeometryType::eBOX:
			DrawBox(geometry);
			break;
		case PxGeometryType::eCAPSULE:
			DrawCapsule(geometry);
			break;
		case PxGeometryType::eCONVEXMESH:
			DrawConvexMesh(geometry);
			break;
		case PxGeometryType::eTRIANGLEMESH:
			DrawTriangleMesh(geometry);
			break;
		default:
			break;
		}
	}

	void RenderCloth(const PxCloth* cloth)
	{
		PxClothMeshDesc* mesh_desc = ((UserData*)cloth->userData)->clothMeshDesc;
		PxVec3* color = ((UserData*)cloth->userData)->color;

		PxU32 quad_count = mesh_desc->quads.count;
		PxU32* quads = (PxU32*)mesh_desc->quads.data;

		vector<PxVec3> verts(cloth->getNbParticles());
		vector<PxVec3> norms(verts.size(), PxVec3(0.f, 0.f, 0.f));

		//get verts data
		cloth->lockParticleData();

		PxClothParticleData* particle_data = cloth->lockParticleData();
		if (!particle_data)
			return;
		// copy vertex positions
		for (PxU32 j = 0; j < verts.size(); j++)
			verts[j] = particle_data->particles[j].pos;

		particle_data->unlock();

		for (PxU32 i = 0; i < quad_count * 4; i += 4)
		{
			PxVec3 v0 = verts[quads[i]];
			PxVec3 v1 = verts[quads[i + 1]];
			PxVec3 v2 = verts[quads[i + 2]];
			PxVec3 n = -((v1 - v0).cross(v2 - v0));

			norms[quads[i]] += n;
			norms[quads[i + 1]] += n;
			norms[quads[i + 2]] += n;
			norms[quads[i + 3]] += n;
		}

		for (PxU32 i = 0; i < norms.size(); i++)
			norms[i].normalize();

		PxTransform pose = cloth->getGlobalPose();
		PxMat44 shapePose(pose);

		glColor4f(color->x, color->y, color->z, 1.f);

		glPushMatrix();
		glMultMatrixf((float*)&shapePose);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		glVertexPointer(3, GL_FLOAT, sizeof(PxVec3), &verts.front());
		glNormalPointer(GL_FLOAT, sizeof(PxVec3), &norms.front());

		glDrawElements(GL_QUADS, quad_count * 4, GL_UNSIGNED_INT, quads);

		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		glPopMatrix();
	}

	// From PhysX Tutorials
	void ResizeWindow(int width, int height)
	{
		glViewport(0, 0, width, height);
		FontRenderer::setScreenResolution(width, height);
	}

	// From PhysX Tutorials
	static void idleCallback()
	{
		//cout << "Idle callback triggered." << endl;
		glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f);
		glutPostRedisplay();
	}

	// From PhysX Tutorials
	void Init(const char* title, int width, int height)
	{
		cout << "Initializing Window with title: " << title << ", width: " << width << ", height: " << height << endl;

		char* titleStr = new char[strlen(title) + 1];
		strcpy_s(titleStr, strlen(title) + 1, title);
		int argc = 1;
		char* argv[1] = { titleStr };

		glutInit(&argc, argv);

		glutInitWindowSize(width, height);
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
		glutSetWindow(glutCreateWindow(title));

		// Debug log to confirm window creation
		cout << "GLUT window created successfully." << endl;

		glutIdleFunc(idleCallback);

		delete[] titleStr;

		// Setup default render states
		PxReal specular_material[] = { .1f, .1f, .1f, 1.f };
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 1.f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material);

		// Setup lighting
		glEnable(GL_LIGHTING);
		PxReal ambientcolor[] = { 0.2f, 0.2f, 0.2f, 1.f };
		PxReal diffusecolor[] = { 0.7f, 0.7f, 0.7f, 1.f };
		PxReal position[] = { 50.f, 50.f, 100.f, 0.f };
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambientcolor);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffusecolor);
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		glEnable(GL_LIGHT0);
	}

	void RenderBuffer(float* pVertList, float* pcolorList, int type, int num)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, pVertList);
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_FLOAT, 0, pcolorList);
		glDrawArrays(type, 0, num);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	void Render(PxActor** actors, const PxU32 numActors)
	{
		PxVec3 shadowColour = defaultColor * .9f;

		for (PxU32 i = 0; i < numActors; i++)
		{
			if (actors[i]->is<PxCloth>())
				RenderCloth((PxCloth*)actors[i]);
			else if (actors[i]->is<PxRigidActor>())
			{
				PxRigidActor* rigidActor = (PxRigidActor*)actors[i];
				vector<PxShape*> shapes(rigidActor->getNbShapes());

				// If the actor has no shapes, skip it
				if (!shapes.size())
					continue;

				rigidActor->getShapes((PxShape**)&shapes.front(), (PxU32)shapes.size());

				for (PxU32 j = 0; j < shapes.size(); j++)
				{
					const PxShape* shape = shapes[j];
					PxTransform pose = PxShapeExt::getGlobalPose(*shape, *shape->getActor());
					PxGeometryHolder geometry = shape->getGeometry();

					if (geometry.getType() == PxGeometryType::ePLANE)
					{
						pose.q *= PxQuat(PxHalfPi, PxVec3(0.f, 0.f, 1.f));
						pose.p += PxVec3(0, -0.01, 0);
					}

					PxMat44 shapePose(pose);

					glPushMatrix();
					glMultMatrixf((float*)&shapePose);

					PxVec3 shapeColour = defaultColor;

					if (shape->userData)
					{
						shapeColour = *((UserData*)shape->userData)->color;
						if (geometry.getType() == PxGeometryType::ePLANE)
							shadowColour = shapeColour * .9f;
					}

					if (geometry.getType() == PxGeometryType::ePLANE)
						glDisable(GL_LIGHTING);

					glColor4f(shapeColour.x, shapeColour.y, shapeColour.z, 1.f);

					RenderGeometry(geometry);

					if (geometry.getType() == PxGeometryType::ePLANE)
						glEnable(GL_LIGHTING);

					glPopMatrix();

					if (renderShadows && (geometry.getType() != PxGeometryType::ePLANE))
					{
						const PxVec3 shadowDir = PxVec3(-0.7071067f, -0.7071067f, -0.7071067f);
						const PxReal shadowMat[] = { 1,0,0,0, -shadowDir.x / shadowDir.y,0,-shadowDir.z / shadowDir.y,0, 0,0,1,0, 0,0,0,1 };

						glPushMatrix();
						glMultMatrixf(shadowMat);
						glMultMatrixf((float*)&shapePose);
						glDisable(GL_LIGHTING);
						glColor4f(shadowColour.x, shadowColour.y, shadowColour.z, 1.f);
						RenderGeometry(geometry);
						glEnable(GL_LIGHTING);
						glPopMatrix();
					}
				}
			}
		}
	}

	void Render(const PxRenderBuffer& data, PxReal lineWidth)
	{
		glLineWidth(lineWidth);

		// Render points

		unsigned int NbPoints = data.getNbPoints();
		if (NbPoints)
		{
			vector<float> pVertList(NbPoints * 3);
			vector<float> pColorList(NbPoints * 4);
			int vertIndex = 0;
			int colorIndex = 0;
			const physx::PxDebugPoint* Points = data.getPoints();
			while (NbPoints--)
			{
				pVertList[vertIndex++] = Points->pos.x;
				pVertList[vertIndex++] = Points->pos.y;
				pVertList[vertIndex++] = Points->pos.z;
				pColorList[colorIndex++] = (float)((Points->color >> 16) & 0xff) / 255.f;
				pColorList[colorIndex++] = (float)((Points->color >> 8) & 0xff) / 255.f;
				pColorList[colorIndex++] = (float)(Points->color & 0xff) / 255.f;
				pColorList[colorIndex++] = 1.f;
				Points++;
			}

			RenderBuffer(&pVertList.front(), &pColorList.front(), GL_POINTS, data.getNbPoints());
		}

		// Render lines

		unsigned int NbLines = data.getNbLines();
		if (NbLines)
		{
			vector<float> pVertList(NbLines * 3 * 2);
			vector<float> pColorList(NbLines * 4 * 2);
			int vertIndex = 0;
			int colorIndex = 0;
			const PxDebugLine* Lines = data.getLines();
			while (NbLines--)
			{
				pVertList[vertIndex++] = Lines->pos0.x;
				pVertList[vertIndex++] = Lines->pos0.y;
				pVertList[vertIndex++] = Lines->pos0.z;
				pColorList[colorIndex++] = (float)((Lines->color0 >> 16) & 0xff) / 255.f;
				pColorList[colorIndex++] = (float)((Lines->color0 >> 8) & 0xff) / 255.f;
				pColorList[colorIndex++] = (float)(Lines->color0 & 0xff) / 255.f;
				pColorList[colorIndex++] = 1.f;

				pVertList[vertIndex++] = Lines->pos1.x;
				pVertList[vertIndex++] = Lines->pos1.y;
				pVertList[vertIndex++] = Lines->pos1.z;
				pColorList[colorIndex++] = (float)((Lines->color1 >> 16) & 0xff) / 255.f;
				pColorList[colorIndex++] = (float)((Lines->color1 >> 8) & 0xff) / 255.f;
				pColorList[colorIndex++] = (float)(Lines->color1 & 0xff) / 255.f;
				pColorList[colorIndex++] = 1.f;

				Lines++;
			}

			RenderBuffer(&pVertList.front(), &pColorList.front(), GL_LINES, data.getNbLines() * 2);
		}

		// Render triangles

		unsigned int NbTris = data.getNbTriangles();
		if (NbTris)
		{
			vector<float> pVertList(NbTris * 3 * 3);
			vector<float> pColorList(NbTris * 4 * 3);
			int vertIndex = 0;
			int colorIndex = 0;
			const PxDebugTriangle* Triangles = data.getTriangles();
			while (NbTris--)
			{
				pVertList[vertIndex++] = Triangles->pos0.x;
				pVertList[vertIndex++] = Triangles->pos0.y;
				pVertList[vertIndex++] = Triangles->pos0.z;

				pVertList[vertIndex++] = Triangles->pos1.x;
				pVertList[vertIndex++] = Triangles->pos1.y;
				pVertList[vertIndex++] = Triangles->pos1.z;

				pVertList[vertIndex++] = Triangles->pos2.x;
				pVertList[vertIndex++] = Triangles->pos2.y;
				pVertList[vertIndex++] = Triangles->pos2.z;

				pColorList[colorIndex++] = (float)((Triangles->color0 >> 16) & 0xff) / 255.f;
				pColorList[colorIndex++] = (float)((Triangles->color0 >> 8) & 0xff) / 255.f;
				pColorList[colorIndex++] = (float)(Triangles->color0 & 0xff) / 255.f;
				pColorList[colorIndex++] = 1.f;

				pColorList[colorIndex++] = (float)((Triangles->color1 >> 16) & 0xff) / 255.f;
				pColorList[colorIndex++] = (float)((Triangles->color1 >> 8) & 0xff) / 255.f;
				pColorList[colorIndex++] = (float)(Triangles->color1 & 0xff) / 255.f;
				pColorList[colorIndex++] = 1.f;

				pColorList[colorIndex++] = (float)((Triangles->color2 >> 16) & 0xff) / 255.f;
				pColorList[colorIndex++] = (float)((Triangles->color2 >> 8) & 0xff) / 255.f;
				pColorList[colorIndex++] = (float)(Triangles->color2 & 0xff) / 255.f;
				pColorList[colorIndex++] = 1.f;

				Triangles++;
			}

			RenderBuffer(&pVertList.front(), &pColorList.front(), GL_TRIANGLES, data.getNbTriangles() * 3);
		}
	}

	void RenderText(const string& text, int x, int y)
	{
		FontRenderer::setColor(1.0f, 1.0f, 1.0f, 1.0f); // Default white color
		FontRenderer::setScreenResolution(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		FontRenderer::print(x, y, 1.0f, text.c_str());
	}

	// From PhysX Tutorials
	void Start(const PxVec3& cameraEye, const PxVec3& cameraDir)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.f, (float)glutGet(GLUT_WINDOW_WIDTH) / (float)glutGet(GLUT_WINDOW_HEIGHT), 1.f, 10000.f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(cameraEye.x, cameraEye.y, cameraEye.z, cameraEye.x + cameraDir.x, cameraEye.y + cameraDir.y, cameraEye.z + cameraDir.z, 0.f, 1.f, 0.f);
	}

	// From PhysX Tutorials
	void End()
	{
		glutSwapBuffers();
	}
}