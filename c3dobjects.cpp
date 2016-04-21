/****************************************************************************
			72 Hour Game Development Contest!

 All code (c) Tim Graupmann 2003, 2004.

 Feel free to use any of this code with only the following condition:

 1) The code is not supported in any way. I am not responsible for any
    harm caused to your machine (although I have made every attemp to check
    for any possibly damaging stuff)
 
 Enjoy! :)

 Credits:
   A lot of my code was based on tutorials by GT (www.gametutorials.com).   

****************************************************************************/

#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>
#include <math.h>


// Include the basic libraries
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")
#pragma comment(lib, "winmm.lib")

// Our custom classes
#include "c3dObjects.h"

// Our game external variables
extern UINT g_Texture[];
extern c3dOrange g_orange;
extern c3dCandyCorn g_CandyCorns[];
extern c3dCandyGobber g_CandyGobbers[];
extern c3dBubbleGum g_BubbleGums[];
extern c3dSpeedBoost g_SpeedBoosts[];
extern double g_MyScore;

// Our vector implementations
float Magnitude(CVector3 vVector)
{
	return sqrtf(vVector.X*vVector.X + vVector.Y*vVector.Y + vVector.Z*vVector.Z);
}
float Magnitude(CVector3 vVector1, CVector3 vVector2)
{
	float dx = vVector1.X - vVector2.X;
	float dy = vVector1.Y - vVector2.Y;
	float dz = vVector1.Z - vVector2.Z;
	return sqrtf(dx*dx + dy*dy + dz*dz);
}

CVector3 Normalize(CVector3 vVector)
{
	float mag = Magnitude(vVector);	
	if(mag)
		return vVector / mag;
	else
		return vVector;
}

CVector3 Cross(CVector3 vVector1, CVector3 vVector2)
{
	CVector3 vNormal;	

	// Calculate the cross product with the non communitive equation
	vNormal.X = ((vVector1.Y * vVector2.Z) - (vVector1.Z * vVector2.Y));
	vNormal.Y = ((vVector1.Z * vVector2.X) - (vVector1.X * vVector2.Z));
	vNormal.Z = ((vVector1.X * vVector2.Y) - (vVector1.Y * vVector2.X));

	// Return the cross product
	return vNormal;										 
}

CVector3::CVector3()
{
	X = Y = Z = 0;
}

CVector3::~CVector3()
{
	X = Y = Z = 0;
}
CVector3::CVector3(const CVector3 & copy)
{
	X = copy.X;
	Y = copy.Y;
	Z = copy.Z;
}
CVector3::CVector3(const float x, const float y, const float z)
{
	X = x;
	Y = y;
	Z = z;
}
CVector3 CVector3::operator + (const CVector3 vVector)
{
	CVector3 temp;
	memset(&temp, 0, sizeof(CVector3));
	temp.X = X + vVector.X;
	temp.Y = Y + vVector.Y;
	temp.Z = Z + vVector.Z;
	return temp;
}
CVector3 CVector3::operator - (const CVector3 vVector)
{
	CVector3 temp;
	memset(&temp, 0, sizeof(CVector3));
	temp.X = X - vVector.X;
	temp.Y = Y - vVector.Y;
	temp.Z = Z - vVector.Z;
	return temp;
}
CVector3 CVector3::operator * (const float Scalar)
{
	CVector3 temp;
	memset(&temp, 0, sizeof(CVector3));
	temp.X = X * Scalar;
	temp.Y = Y * Scalar;
	temp.Z = Z * Scalar;
	return temp;
}
CVector3 CVector3::operator / (const float Scalar)
{
	CVector3 temp;
	memset(&temp, 0, sizeof(CVector3));
	temp.X = X / Scalar;
	temp.Y = Y / Scalar;
	temp.Z = Z / Scalar;
	return temp;
}


c3dObject::c3dObject()
{
	m_vPosition	= CVector3(0.0, 0.0, 0.0);
	m_vView		= CVector3(0.0, 0.0, 1.0);
	m_vUpVector	= CVector3(0.0, 1.0, 0.0);
	m_oType = TYPE_NONE;
	m_TextureID = -1;
}

c3dObject::c3dObject(float positionX, float positionY, float positionZ,
					 float viewX,     float viewY,     float viewZ,
					 float upVectorX, float upVectorY, float upVectorZ,
					 UINT textureID)
{
	m_oType = TYPE_NONE;
	PositionObject(positionX, positionY, positionZ,
		           viewX,     viewY,     viewZ,
				   upVectorX, upVectorY, upVectorZ);
	m_TextureID = textureID;
}

void c3dObject::PositionObject(float positionX, float positionY, float positionZ,
				  		  float viewX,     float viewY,     float viewZ,
						  float upVectorX, float upVectorY, float upVectorZ)
{
	m_vPosition = CVector3(positionX, positionY, positionZ);
	m_vView     = CVector3(viewX, viewY, viewZ);
	m_vUpVector = CVector3(upVectorX, upVectorY, upVectorZ);
}

void c3dObject::MoveObject(float speed)
{
	// Get the current view vector (the direction we are looking)
	CVector3 vVector = Normalize(m_vView - m_vPosition);
	CVector3 newPosition = m_vPosition + vVector * speed;
	
	m_vPosition = newPosition;	
	m_vView.X += vVector.X * speed;			// Add our acceleration to our view's X	
	m_vView.Y += vVector.Y * speed;			// Add our acceleration to our view's Y
	m_vView.Z += vVector.Z * speed;			// Add our acceleration to our view's Z
}

void c3dObject::RotateView(float angle, float x, float y, float z)
{
	CVector3 vNewView;

	// Get the view vector (The direction we are facing)
	CVector3 vView = m_vView - m_vPosition;		

	// Calculate the sine and cosine of the angle once
	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	// Find the new x position for the new rotated point
	vNewView.X  = (cosTheta + (1 - cosTheta) * x * x)		* vView.X;
	vNewView.X += ((1 - cosTheta) * x * y - z * sinTheta)	* vView.Y;
	vNewView.X += ((1 - cosTheta) * x * z + y * sinTheta)	* vView.Z;

	// Find the new y position for the new rotated point
	vNewView.Y  = ((1 - cosTheta) * x * y + z * sinTheta)	* vView.X;
	vNewView.Y += (cosTheta + (1 - cosTheta) * y * y)		* vView.Y;
	vNewView.Y += ((1 - cosTheta) * y * z - x * sinTheta)	* vView.Z;

	// Find the new z position for the new rotated point
	vNewView.Z  = ((1 - cosTheta) * x * z - y * sinTheta)	* vView.X;
	vNewView.Z += ((1 - cosTheta) * y * z + x * sinTheta)	* vView.Y;
	vNewView.Z += (cosTheta + (1 - cosTheta) * z * z)		* vView.Z;
	
	// Now we just add the newly rotated vector to our position to set
	// our new rotated view of our camera.
	m_vView = m_vPosition + vNewView;
}

void c3dObject::RotateUpView(float angle, float x, float y, float z)
{
	// The new vector to store data in
	CVector3 vNewView;

	// The origin
	CVector3 vOrigin;

	// Get the up view vector (The direction that points up from the camera)
	CVector3 vView = m_vUpVector - vOrigin;	

	// Calculate the sine and cosine of the angle once
	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	// Find the new x position for the new rotated point
	vNewView.X  = (cosTheta + (1 - cosTheta) * x * x)		* vView.X;
	vNewView.X += ((1 - cosTheta) * x * y - z * sinTheta)	* vView.Y;
	vNewView.X += ((1 - cosTheta) * x * z + y * sinTheta)	* vView.Z;

	// Find the new y position for the new rotated point
	vNewView.Y  = ((1 - cosTheta) * x * y + z * sinTheta)	* vView.X;
	vNewView.Y += (cosTheta + (1 - cosTheta) * y * y)		* vView.Y;
	vNewView.Y += ((1 - cosTheta) * y * z - x * sinTheta)	* vView.Z;

	// Find the new z position for the new rotated point
	vNewView.Z  = ((1 - cosTheta) * x * z - y * sinTheta)	* vView.X;
	vNewView.Z += ((1 - cosTheta) * y * z + x * sinTheta)	* vView.Y;
	vNewView.Z += (cosTheta + (1 - cosTheta) * z * z)		* vView.Z;

	// Now we just add the newly rotated vector to our position to set
	// our new rotated view of our camera.
	m_vUpVector = vNewView;
}

void c3dObject::RollObject(float speed)
{
	// Cross the cross product
	CVector3 v_yAxis = Cross( Cross(m_vView - m_vPosition, m_vUpVector), m_vUpVector);
	v_yAxis = Normalize(v_yAxis);

	// Rotate around the y axis no matter what the currentRotX is
	RotateView(speed, v_yAxis.X, v_yAxis.Y, v_yAxis.Z);

	// Rotate the up view by the same amount
	RotateUpView(speed, v_yAxis.X, v_yAxis.Y, v_yAxis.Z);
}


void c3dCamera::Look(void)
{
	// Put our camera in place
	gluLookAt(m_vPosition.X, m_vPosition.Y, m_vPosition.Z,	
			  m_vView.X,	 m_vView.Y,     m_vView.Z,	
			  m_vUpVector.X, m_vUpVector.Y, m_vUpVector.Z);
}


c3dOrange::c3dOrange(float positionX, float positionY, float positionZ,
					 float viewX,     float viewY,     float viewZ,
					 float upVectorX, float upVectorY, float upVectorZ,
                     UINT textureID, float RotationX, float RollSpeed,
					 float HeightOffset, float Radius)
{
	m_oType = TYPE_ORANGE;
	PositionObject(positionX, positionY, positionZ,
		           viewX,     viewY,     viewZ,
				   upVectorX, upVectorY, upVectorZ);
	m_TextureID = textureID;

	m_State = 2;
	m_Radius = Radius;
	m_BubbleRadius = 0;
	m_RotationX = RotationX;
	m_RollSpeed = RollSpeed;
	m_HeightOffset = HeightOffset;
	m_TimeToJump = 0;
	m_LastJumpTime = GetTickCount()-5000;
	m_TimeToSpeed = 0;
	m_LastSpeedTime = GetTickCount()-5000;
	m_TimeToPenalty = 0;
	m_LastPenaltyTime = GetTickCount()-5000;
	m_TimeToBubble = 0;
	m_LastBubbleTime = GetTickCount()-5000;
	m_Modified = GetTickCount();
	m_BubbleQty = 0;
	m_SpeedBoostQty = 0;
	m_LastOpponent = TYPE_NONE;
}

void c3dOrange::Update(void)
{
	if(!m_State)
		return;

	// Handle keyboard input -->

	// Change the texture
	if(GetKeyState('1') & 0x80)
		m_TextureID = 0;
	else if(GetKeyState('2') & 0x80)
		m_TextureID = 1;
	
	// Roll the orange
	if( (GetKeyState(VK_UP) & 0x80) || (GetKeyState('W') & 0x80) )
	{
		if(m_RollSpeed < 5.0f)
			m_RollSpeed += 0.1f;
	}
	else if( (GetKeyState(VK_DOWN) & 0x80) || (GetKeyState('S') & 0x80) )
	{
		if(m_RollSpeed > -5.0f)
			m_RollSpeed -= 0.1f;
	}
	else
		m_RollSpeed *= 0.99f;

	// Turn the orange
	if( (GetKeyState(VK_RIGHT) & 0x80) || (GetKeyState('D') & 0x80) )
	{
		CVector3 vView = Normalize( m_vView - m_vPosition );
		CVector3 vStrafe = Normalize( Cross(m_vView-m_vPosition, m_vUpVector) );
		CVector3 vUpStrafe = Normalize( Cross(vStrafe, vView) );
		RotateView(-0.05f, vUpStrafe.X, vUpStrafe.Y, vUpStrafe.Z);
		RotateUpView(-0.05f, vUpStrafe.X, vUpStrafe.Y, vUpStrafe.Z);
	}
	else if( (GetKeyState(VK_LEFT) & 0x80) || (GetKeyState('A') & 0x80) )
	{
		CVector3 vView = Normalize( m_vView - m_vPosition );
		CVector3 vStrafe = Normalize( Cross(m_vView-m_vPosition, m_vUpVector) );
		CVector3 vUpStrafe = Normalize( Cross(vStrafe, vView) );		
		RotateView(+0.05f, vUpStrafe.X, vUpStrafe.Y, vUpStrafe.Z);
		RotateUpView(+0.05f, vUpStrafe.X, vUpStrafe.Y, vUpStrafe.Z);
	}
	
	if(GetKeyState(VK_SPACE) & 0x80)
	{
		// Jump the orange
		if(!m_TimeToJump)
		{
			if( labs(GetTickCount() - m_LastJumpTime) >= 750 )
			{
				m_TimeToJump = 1000;
				m_LastJumpTime = GetTickCount();
				if(!m_TimeToSpeed)
				{
					m_TimeToSpeed = 1000;
					m_LastSpeedTime = GetTickCount();
				}
			}
		}

		// Bubble the orange
		if(!m_TimeToBubble)
		{
			if(m_BubbleQty)
			{
				m_BubbleQty--;
				m_TimeToBubble = 10000;
				m_LastBubbleTime = GetTickCount();
				m_BubbleRadius = m_Radius;
			}
		}
	}	

	// Handle player enhancements -->

	// Penalty the orange
	if(m_TimeToPenalty)
	{
		m_Radius *= 0.99f; // Shrink the orange

		// Expire the penalty
		if( labs(GetTickCount() - m_LastPenaltyTime) >= m_TimeToPenalty )
			m_TimeToPenalty = 0;

		if(m_TimeToBubble) // Get rid of its bubble
		{
			m_TimeToBubble = 0;
			m_BubbleRadius = 0;
		}
		if(m_TimeToSpeed)  // Get rid of the speed enhancement
			m_TimeToSpeed = 0;

		// Expire the speed boosts
		m_SpeedBoostQty = 0;
	}
	else
	{
		if(m_TimeToBubble)
		{
			// Grow the bubble
			if(m_BubbleRadius < (2*m_Radius) )
				m_BubbleRadius += 0.01f;
			
			// Expire the bubble
			if( labs(GetTickCount() - m_LastBubbleTime) >= m_TimeToBubble )
			{
				m_TimeToBubble = 0;
				m_BubbleRadius = 0;
			}
		}
		if(m_TimeToSpeed)
		{
			// Expire the speed
			if( labs(GetTickCount() - m_LastSpeedTime) >= m_TimeToSpeed )
			{
				m_TimeToSpeed = 0;
			}
		}
		if(m_TimeToJump)
		{
			// Expire the jump
			if( labs(GetTickCount() - m_LastJumpTime) >= m_TimeToJump )
			{
				m_TimeToJump = 0;
				m_LastJumpTime = GetTickCount();
			}

			// Raise the orange
			m_HeightOffset = m_HeightOffset * 1.5f + 0.01f;
			if(m_HeightOffset > (2*m_Radius)) // Cap its height
				m_HeightOffset = 2*m_Radius;
		}
		else
		{
			m_HeightOffset *= 0.5f; // Lower the orange
		}
	}

	// Roll the orange
	m_RotationX += m_RollSpeed;
	if(m_RotationX >= 360)
		m_RotationX -= 360;
	else if(m_RotationX <= -360)
		m_RotationX += 360;

	// Kill the orange
	if(m_Radius <= 0.4f)
	{
		m_State = 0;
		m_Modified = GetTickCount();
		m_Radius = 0.4f;
	}

	// Cap bubble growth
	if(m_BubbleRadius > (m_Radius*2))
		m_BubbleRadius = m_Radius*2;

	// Cap speed boosts
	if(m_SpeedBoostQty > 5)
		m_SpeedBoostQty = 5;

	// Place the base position of the orange on the ground
	m_vPosition.Y = m_Radius;
	m_vView.Y = m_vPosition.Y;

	// Move the orange
	if(m_TimeToSpeed && m_SpeedBoostQty)
		MoveObject(m_RollSpeed * 0.01f * m_SpeedBoostQty);
	else
		MoveObject(m_RollSpeed * 0.01f);
}

void c3dOrange::Render(void)
{
	if(!m_State)
		return;

	glPushMatrix();


	glTranslatef(m_vPosition.X, m_vPosition.Y+m_HeightOffset, m_vPosition.Z);
	
	// Keep orange rolling along the strafe vector
	CVector3 vView = Normalize( m_vView - m_vPosition );
	CVector3 vStrafe = Normalize( Cross(m_vView-m_vPosition, m_vUpVector) );
	glRotatef(-m_RotationX, vStrafe.X, vStrafe.Y, vStrafe.Z);

	// Draw our orange
	if(m_TimeToPenalty)
		glColor3ub(rand()%256, rand()%256, rand()%256);
	else
		glColor3ub(255, 255, 255);
	glBindTexture(GL_TEXTURE_2D, g_Texture[m_TextureID]);
	GLUquadric * quad;
	quad = gluNewQuadric();
	gluQuadricTexture(quad, GL_TRUE);
	gluQuadricOrientation(quad, GLU_OUTSIDE);
	gluSphere(quad, m_Radius, 24, 24);

	if(m_BubbleRadius)
	{
		if(labs(GetTickCount() - m_LastBubbleTime) <= (m_TimeToBubble-2000))
			glColor4ub(255, 200, 200, 128);
		else if(rand()%2)
			glColor4ub(255, 200, 200, 64);
		else
			glColor4ub(255, 200, 200, 32);
		
		glBindTexture(GL_TEXTURE_2D, g_Texture[3]);
		gluSphere(quad, m_BubbleRadius, 24, 24);
	}
	gluDeleteQuadric(quad);

	glColor3ub(255, 255, 255);


	glPopMatrix();
}


c3dBubbleGum::c3dBubbleGum(float positionX, float positionY, float positionZ,
						   float viewX,     float viewY,     float viewZ,
						   float upVectorX, float upVectorY, float upVectorZ,
                           UINT textureID, float Rotation,
						   float HeightOffset, short State)
{
	m_oType = TYPE_BUBBLEGUM;
	PositionObject(positionX, positionY, positionZ,
		           viewX,     viewY,     viewZ,
				   upVectorX, upVectorY, upVectorZ);
	m_TextureID = textureID;
	m_Rotation = Rotation;
	m_HeightOffset = HeightOffset;
	m_State = State;
	m_Score = 0;
}

void c3dBubbleGum::Update(void)
{
	switch(m_State)
	{
	case 0: // Not Alive
		{
			// Make sure that we place the bubble gum away from everything
			bool loop = 1;
			while(loop)
			{
				m_vPosition = CVector3((rand()%40)-20.0f, 1, (rand()%40)-20.0f);
				loop = 0;

				// Check proximity to Orange
				if(Magnitude(m_vPosition, g_orange.m_vPosition) < 1)
					loop = 1;
				if(loop == 1)
					continue;

				// Check proximity to Candy Corns
				for(UINT i=0; i<MAX_CANDYCORNS; i++)
				{
					if(Magnitude(m_vPosition, g_CandyCorns[i].m_vPosition) < 1)
					{
						loop = 1;
						break;
					}
				}
				if(loop == 1)
					continue;
				
				// Check proximity to CandyGobbers
				for(i=0; i<MAX_CANDYGOBBERS; i++)
				{
					if(Magnitude(m_vPosition, g_CandyGobbers[i].m_vPosition) < 1)
					{
						loop = 1;
						break;
					}
				}
				if(loop == 1)
					continue;

				// Check proximity to other Bubble Gums
				for(i=0; i<MAX_BUBBLEGUMS; i++)
				{
					if((&g_BubbleGums[i]) == this)
						continue;
					if(Magnitude(m_vPosition, g_BubbleGums[i].m_vPosition) < 1)
					{
						loop = 1;
						break;
					}
				}
				if(loop == 1)
					continue;

				// Check proximity to other Bubble Gums
				for(i=0; i<MAX_SPEEDBOOSTS; i++)
				{
					if(Magnitude(m_vPosition, g_SpeedBoosts[i].m_vPosition) < 1)
					{
						loop = 1;
						break;
					}
				}
				if(loop == 1)
					continue;
			}
		}
		m_State = 2;
		break;
	case 2: // Alive
		m_Rotation += 1.5f;
		if(m_Rotation > 360)
			m_Rotation -= 360;
		if(!g_orange.m_State)
			break;
		if( Magnitude( m_vPosition, g_orange.m_vPosition + CVector3(0, g_orange.m_HeightOffset, 0) ) <= (g_orange.m_Radius+0.3f) )
		{
			// Bubble the orange
			g_orange.m_BubbleQty++;
			g_orange.m_TimeToBubble = 10000;
			g_orange.m_LastBubbleTime = GetTickCount();
			g_orange.m_BubbleRadius = g_orange.m_Radius;
			m_State = 0;
			g_MyScore += m_Score;
			g_orange.m_LastOpponent = m_oType;
		}
		break;
	}
}

void c3dBubbleGum::Render(void)
{
	if(!m_State)
		return;

	glPushMatrix();	
	
	glTranslatef(m_vPosition.X, m_vPosition.Y + m_HeightOffset, m_vPosition.Z);
	glRotatef(m_Rotation, 1, 0.5f, 0);

	// Draw our candy corn
	glBindTexture(GL_TEXTURE_2D, g_Texture[m_TextureID]);
	
	// Bottom of candy
	glBegin(GL_QUADS);
	glColor3ub(255, 255, 255);
	glTexCoord2f(0, 1);			glVertex3f(-1,	0,	0.5f);
	glTexCoord2f(0, 0);			glVertex3f(-1,	0,	-0.5f);
	glTexCoord2f(1, 0);			glVertex3f(+1,	0,	-0.5f);
	glTexCoord2f(1, 1);			glVertex3f(+1,	0,	0.5f);
	glEnd();

	glPopMatrix();
}


c3dSpeedBoost::c3dSpeedBoost(float positionX, float positionY, float positionZ,
							 float viewX,     float viewY,     float viewZ,
							 float upVectorX, float upVectorY, float upVectorZ,
                             UINT textureID, float Rotation,
							 float HeightOffset, short State)
{
	m_oType = TYPE_SPEEDBOOST;
	PositionObject(positionX, positionY, positionZ,
		           viewX,     viewY,     viewZ,
				   upVectorX, upVectorY, upVectorZ);
	m_TextureID = textureID;
	m_Rotation = Rotation;
	m_HeightOffset = HeightOffset;
	m_State = State;
	m_Score = 0;
}

void c3dSpeedBoost::Update(void)
{
	switch(m_State)
	{
	case 0: // Not Alive
		// Make sure that we place the speed boost away from everything
		{
			bool loop = 1;
			while(loop)
			{
				m_vPosition = CVector3((rand()%40)-20.0f, 1, (rand()%40)-20.0f);
				loop = 0;

				// Check proximity to Orange
				if(Magnitude(m_vPosition, g_orange.m_vPosition) < 1)
					loop = 1;
				if(loop == 1)
					continue;

				// Check proximity to Candy Corns
				for(UINT i=0; i<MAX_CANDYCORNS; i++)
				{
					if(Magnitude(m_vPosition, g_CandyCorns[i].m_vPosition) < 1)
					{
						loop = 1;
						break;
					}
				}
				if(loop == 1)
					continue;
				
				// Check proximity to CandyGobbers
				for(i=0; i<MAX_CANDYGOBBERS; i++)
				{
					if(Magnitude(m_vPosition, g_CandyGobbers[i].m_vPosition) < 1)
					{
						loop = 1;
						break;
					}
				}
				if(loop == 1)
					continue;

				// Check proximity to other Bubble Gums
				for(i=0; i<MAX_BUBBLEGUMS; i++)
				{
					if(Magnitude(m_vPosition, g_BubbleGums[i].m_vPosition) < 1)
					{
						loop = 1;
						break;
					}
				}
				if(loop == 1)
					continue;

				// Check proximity to other Bubble Gums
				for(i=0; i<MAX_SPEEDBOOSTS; i++)
				{
					if((&g_SpeedBoosts[i]) == this)
						continue;
					if(Magnitude(m_vPosition, g_SpeedBoosts[i].m_vPosition) < 1)
					{
						loop = 1;
						break;
					}
				}
				if(loop == 1)
					continue;
			}
		}
		m_State = 2;
		break;
	case 2: // Alive
		m_Rotation += 1.5f;
		if(m_Rotation > 360)
			m_Rotation -= 360;
		if(!g_orange.m_State)
			break;
		if( Magnitude( m_vPosition, g_orange.m_vPosition + CVector3(0, g_orange.m_HeightOffset, 0) ) <= (g_orange.m_Radius+0.3f) )
		{
			g_orange.m_TimeToSpeed = 10000;
			g_orange.m_LastSpeedTime = GetTickCount();
			g_orange.m_SpeedBoostQty++;
			m_State = 0;
			g_MyScore += m_Score;
			g_orange.m_LastOpponent = m_oType;
		}
		break;
	}
}

void c3dSpeedBoost::Render(void)
{
	if(!m_State)
		return;

	glPushMatrix();	
	
	glTranslatef(m_vPosition.X, m_vPosition.Y + m_HeightOffset, m_vPosition.Z);
	glRotatef(m_Rotation, 1, 0.5f, 0);

	// Draw our candy corn
	glBindTexture(GL_TEXTURE_2D, g_Texture[m_TextureID]);
	
	// Bottom of candy
	glBegin(GL_QUADS);
	glColor3ub(255, 255, 255);
	glTexCoord2f(0, 1);			glVertex3f(-1,	0,	0.5f);
	glTexCoord2f(0, 0);			glVertex3f(-1,	0,	-0.5f);
	glTexCoord2f(1, 0);			glVertex3f(+1,	0,	-0.5f);
	glTexCoord2f(1, 1);			glVertex3f(+1,	0,	0.5f);
	glEnd();

	glPopMatrix();
}


c3dCandyCorn::c3dCandyCorn(float positionX, float positionY, float positionZ,
						   float viewX,     float viewY,     float viewZ,
						   float upVectorX, float upVectorY, float upVectorZ,
                           UINT textureID, float Rotation,
						   float HeightOffset, short State)
{
	m_oType = TYPE_CANDYCORN;
	PositionObject(positionX, positionY, positionZ,
		           viewX,     viewY,     viewZ,
				   upVectorX, upVectorY, upVectorZ);
	m_TextureID = textureID;
	m_Rotation = Rotation;	
	m_HeightOffset = HeightOffset;
	m_BubbleRadius = 0;
	m_State = State;
	m_Score = 5;
}

void c3dCandyCorn::Update(void)
{
	switch(m_State)
	{
	case 0: // Not Alive
		// Make sure that we place the candy corn away from everything
		{
			bool loop = 1;
			while(loop)
			{
				m_vPosition = CVector3((rand()%40)-20.0f, 0, (rand()%40)-20.0f);
				m_vView = CVector3(m_vPosition.X, 100, m_vPosition.Z);
				loop = 0;

				// Check proximity to Orange
				if(Magnitude(m_vPosition, g_orange.m_vPosition) < 1)
					loop = 1;
				if(loop == 1)
					continue;

				// Check proximity to Candy Corns
				for(UINT i=0; i<MAX_CANDYCORNS; i++)
				{
					if((&g_CandyCorns[i]) == this)
						continue;
					if(Magnitude(m_vPosition, g_CandyCorns[i].m_vPosition) < 1)
					{
						loop = 1;
						break;
					}
				}
				if(loop == 1)
					continue;
				
				// Check proximity to CandyGobbers
				for(i=0; i<MAX_CANDYGOBBERS; i++)
				{
					if(Magnitude(m_vPosition, g_CandyGobbers[i].m_vPosition) < 1)
					{
						loop = 1;
						break;
					}
				}
				if(loop == 1)
					continue;

				// Check proximity to other Bubble Gums
				for(i=0; i<MAX_BUBBLEGUMS; i++)
				{
					if(Magnitude(m_vPosition, g_BubbleGums[i].m_vPosition) < 1)
					{
						loop = 1;
						break;
					}
				}
				if(loop == 1)
					continue;

				// Check proximity to other Bubble Gums
				for(i=0; i<MAX_SPEEDBOOSTS; i++)
				{
					if(Magnitude(m_vPosition, g_SpeedBoosts[i].m_vPosition) < 1)
					{
						loop = 1;
						break;
					}
				}
				if(loop == 1)
					continue;
			}
		}
		m_BubbleRadius = 0;
		m_State = 2;
		break;
	case 1: // Inactive
		if(m_vPosition.Y > 8)
			m_State = 0;
		else
			MoveObject(0.05f);
		break;
	case 2: // Alive
		m_Rotation += 1.5f;
		if(m_Rotation > 360)
			m_Rotation -= 360;
		if(!g_orange.m_State)
			break;
		if( Magnitude( m_vPosition, g_orange.m_vPosition + CVector3(0, g_orange.m_HeightOffset, 0) ) <= (g_orange.m_BubbleRadius+0.3f) )
		{
			// put a bubble around the candy and take off
			m_BubbleRadius = g_orange.m_Radius;
			g_MyScore += m_Score;
			m_State = 1;
			g_orange.m_LastOpponent = m_oType;
		}
		else if( Magnitude( m_vPosition, g_orange.m_vPosition + CVector3(0, g_orange.m_HeightOffset, 0) ) <= (g_orange.m_Radius+0.3f) )
		{
			g_orange.m_TimeToPenalty = 100;
			g_orange.m_LastPenaltyTime = GetTickCount();
			g_orange.m_LastOpponent = m_oType;
		}
	}
}

void c3dCandyCorn::Render(void)
{
	if(!m_State)
		return;

	glPushMatrix();	
	
	glTranslatef(m_vPosition.X, m_vPosition.Y + m_HeightOffset, m_vPosition.Z);
	glRotatef(m_Rotation, 0, 1, 0);

	// Draw our candy corn
	glBindTexture(GL_TEXTURE_2D, g_Texture[m_TextureID]);
	
	float hincr = 0.05f;
	float vincr = 0.2f;

	// 120 degree for each side
	for(UINT index=0; index<4; index++)
	{
		// Top tier of candy
		glBegin(GL_TRIANGLES);
			glColor3ub(255, 255, 255);		
			glTexCoord2f(0.5f, 0.5f);	glVertex3f(0,			3*vincr, 0);
			glTexCoord2f(0, 0);			glVertex3f(-1*hincr,	2*vincr, 1*hincr);
			glTexCoord2f(1, 0);			glVertex3f(+1*hincr,	2*vincr, 1*hincr);
		glEnd();

		// Middle tier of candy
		glBegin(GL_POLYGON);
		glColor3ub(255, 255, 0);
		glTexCoord2f(0, 1);			glVertex3f(-1*hincr,	2*vincr,	1*hincr);
		glTexCoord2f(0, 0);			glVertex3f(-2*hincr,	1*vincr,	2*hincr);
		glTexCoord2f(1, 0);			glVertex3f(+2*hincr,	1*vincr,	2*hincr);
		glTexCoord2f(1, 1);			glVertex3f(+1*hincr,	2*vincr,	1*hincr);
		glEnd();

		// Bottom tier of candy
		glBegin(GL_POLYGON);
		glColor3ub(255, 127, 0);
		glTexCoord2f(0, 1);			glVertex3f(-2*hincr,	1*vincr,	2*hincr);
		glTexCoord2f(0, 0);			glVertex3f(-3*hincr,	0*vincr,	3*hincr);
		glTexCoord2f(1, 0);			glVertex3f(+3*hincr,	0*vincr,	3*hincr);
		glTexCoord2f(1, 1);			glVertex3f(+2*hincr,	1*vincr,	2*hincr);
		glEnd();

		glRotatef(90, 0, 1, 0);
	}

	// Bottom of candy
	glBegin(GL_QUADS);
	glColor3ub(255, 127, 0);
	glTexCoord2f(0, 1);			glVertex3f(-3*hincr,	0*vincr,	+3*hincr);
	glTexCoord2f(0, 0);			glVertex3f(-3*hincr,	0*vincr,	-3*hincr);
	glTexCoord2f(1, 0);			glVertex3f(+3*hincr,	0*vincr,	-3*hincr);
	glTexCoord2f(1, 1);			glVertex3f(+3*hincr,	0*vincr,	+3*hincr);
	glEnd();

	// Draw our bubble
	if(m_BubbleRadius)
	{
		GLUquadric * quad;
		quad = gluNewQuadric();
		gluQuadricTexture(quad, GL_TRUE);
		gluQuadricOrientation(quad, GLU_OUTSIDE);
		glColor4ub(255, 200, 200, 128);
		glBindTexture(GL_TEXTURE_2D, g_Texture[3]);
		gluSphere(quad, m_BubbleRadius, 24, 24);
		gluDeleteQuadric(quad);
	}	

	glColor3ub(255, 255, 255);

	glPopMatrix();
}


c3dCandyGobber::c3dCandyGobber(float positionX, float positionY, float positionZ,
							   float viewX,     float viewY,     float viewZ,
							   float upVectorX, float upVectorY, float upVectorZ,
							   UINT textureID, float Rotation,
							   float HeightOffset, float Radius, short State)
{
	m_oType = TYPE_CANDYGOBBER;
	PositionObject(positionX, positionY, positionZ,
		           viewX,     viewY,     viewZ,
				   upVectorX, upVectorY, upVectorZ);
	m_TextureID = textureID;
	m_Rotation = Rotation;	
	m_HeightOffset = HeightOffset;
	m_Radius = Radius;
	m_BubbleRadius = 0;
	m_State = State;
	m_Color = CVector3(rand()%50/99.0f, rand()%100/99.0f, rand()%100/99.0f);
	m_Score = 25;
}

void c3dCandyGobber::Update(void)
{
	switch(m_State)
	{
	case 0: // Not Alive
		// Make sure that we place the candy corn away from everything
		{
			bool loop = 1;
			while(loop)
			{
				m_vPosition = CVector3((rand()%40)-20.0f, m_Radius, (rand()%40)-20.0f);
				m_vView = CVector3(m_vPosition.X, m_vPosition.Y, m_vPosition.Z+1);
				loop = 0;

				// Check proximity to Orange
				if(Magnitude(m_vPosition, g_orange.m_vPosition) < 1)
					loop = 1;
				if(loop == 1)
					continue;

				// Check proximity to Candy Corns
				for(UINT i=0; i<MAX_CANDYCORNS; i++)
				{
					if(Magnitude(m_vPosition, g_CandyCorns[i].m_vPosition) < 1)
					{
						loop = 1;
						break;
					}
				}
				if(loop == 1)
					continue;
				
				// Check proximity to CandyGobbers
				for(i=0; i<MAX_CANDYGOBBERS; i++)
				{
					if((&g_CandyGobbers[i]) == this)
						continue;
					if(Magnitude(m_vPosition, g_CandyGobbers[i].m_vPosition) < 1)
					{
						loop = 1;
						break;
					}
				}
				if(loop == 1)
					continue;

				// Check proximity to other Bubble Gums
				for(i=0; i<MAX_BUBBLEGUMS; i++)
				{
					if(Magnitude(m_vPosition, g_BubbleGums[i].m_vPosition) < 1)
					{
						loop = 1;
						break;
					}
				}
				if(loop == 1)
					continue;

				// Check proximity to other Bubble Gums
				for(i=0; i<MAX_SPEEDBOOSTS; i++)
				{
					if(Magnitude(m_vPosition, g_SpeedBoosts[i].m_vPosition) < 1)
					{
						loop = 1;
						break;
					}
				}
				if(loop == 1)
					continue;
			}
		}
		m_BubbleRadius = 0;
		m_State = 2;
		break;
	case 1: // Inactive
		MoveObject(0.05f);
		if(m_vPosition.Y > 8)
			m_State = 0;
		break;
	case 2: // Alive
		{
			float speed = 0.1f;
			
			// Find the gobbers new position, if that position is
			// closer to the player then move there, otherwise turn
			CVector3 vVector = Normalize(m_vView - m_vPosition);
			CVector3 newPosition = m_vPosition + vVector * speed;

			bool DontMove = false;

			// Check proximity to orange
			if(Magnitude(newPosition, g_orange.m_vPosition) >= Magnitude(m_vPosition, g_orange.m_vPosition))
				DontMove = true;

			// Check proximity to candy corns
			for(UINT i=0; i<MAX_CANDYCORNS; i++)
			{
				if(Magnitude(newPosition, g_CandyCorns[i].m_vPosition) <= 1.0f)
				{
					//quickly check the current position
					if(Magnitude(m_vPosition, g_CandyCorns[i].m_vPosition) <= 1.0f)
						DontMove = false;
					else
						DontMove = true;
				}
			}

			// Check proximity to other gobbers
			for(i=0; i<MAX_CANDYGOBBERS; i++)
			{
				if((&g_CandyGobbers[i]) == this)
					continue;
				if(Magnitude(newPosition, g_CandyGobbers[i].m_vPosition) <= 1.0f)
				{
					//quickly check the current position
					if(Magnitude(m_vPosition, g_CandyGobbers[i].m_vPosition) <= 1.0f)
						DontMove = false;
					else
						DontMove = true;
				}
			}

			if(DontMove)
			{
				CVector3 vView = Normalize( m_vView - m_vPosition );
				CVector3 vStrafe = Normalize( Cross(m_vView-m_vPosition, m_vUpVector) );
				CVector3 vUpStrafe = Normalize( Cross(vStrafe, vView) );		
				RotateView(0.5f, vUpStrafe.X, vUpStrafe.Y, vUpStrafe.Z);
				RotateUpView(0.5f, vUpStrafe.X, vUpStrafe.Y, vUpStrafe.Z);
			}
			else
				MoveObject(speed);
	
			m_Rotation += 1.5f;
			if(m_Rotation > 360)
				m_Rotation -= 360;
			if(!g_orange.m_State)
				break;
			if(g_orange.m_BubbleRadius)
			{
				if( Magnitude( m_vPosition, g_orange.m_vPosition + CVector3(0, g_orange.m_HeightOffset, 0) ) <= (g_orange.m_BubbleRadius+0.3f) )
				{
					// put a bubble around the candy and take off
					m_BubbleRadius = g_orange.m_Radius;
					m_vView = CVector3(m_vPosition.X, m_vPosition.Y + 20, m_vPosition.Z);
					g_MyScore += m_Score;
					m_State = 1;
					g_orange.m_LastOpponent = m_oType;
				}
			}
			else if( Magnitude( m_vPosition, g_orange.m_vPosition + CVector3(0, g_orange.m_HeightOffset, 0) ) <= (g_orange.m_Radius+0.3f) )
			{
				g_orange.m_TimeToPenalty = 100;
				g_orange.m_LastPenaltyTime = GetTickCount();
				g_orange.m_LastOpponent = m_oType;
			}
		}
	}
}

void c3dCandyGobber::Render(void)
{
	if(!m_State)
		return;

	glPushMatrix();


	glTranslatef(m_vPosition.X, m_vPosition.Y+m_HeightOffset, m_vPosition.Z);
	
	// Keep orange rolling along the strafe vector
	CVector3 vView = Normalize( m_vView - m_vPosition );
	CVector3 vStrafe = Normalize( Cross(m_vView-m_vPosition, m_vUpVector) );
	glRotatef(-m_Rotation, vStrafe.X, vStrafe.Y, vStrafe.Z);

	// Draw our gobber
	glColor3f(m_Color.X, m_Color.Y, m_Color.Z);
	glBindTexture(GL_TEXTURE_2D, g_Texture[m_TextureID]);
	GLUquadric * quad;
	quad = gluNewQuadric();
	gluQuadricTexture(quad, GL_TRUE);
	gluQuadricOrientation(quad, GLU_OUTSIDE);
	gluSphere(quad, m_Radius, 24, 24);

	// Draw our bubble
	if(m_BubbleRadius)
	{
		gluQuadricOrientation(quad, GLU_OUTSIDE);
		glColor4ub(255, 200, 200, 128);
		glBindTexture(GL_TEXTURE_2D, g_Texture[3]);
		gluSphere(quad, 1.5f * m_Radius, 24, 24);
	}
	gluDeleteQuadric(quad);

	glColor3ub(255, 255, 255);


	glPopMatrix();
}