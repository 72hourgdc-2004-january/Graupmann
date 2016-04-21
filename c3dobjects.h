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

#ifndef _C3DOBJECT_H
#define _C3DOBJECT_H


#define MAX_CANDYCORNS		20
#define MAX_CANDYGOBBERS	10
#define MAX_BUBBLEGUMS		5
#define MAX_SPEEDBOOSTS		5

#define TYPE_NONE			-1
#define TYPE_ORANGE			0
#define TYPE_CANDYCORN		1
#define TYPE_CANDYGOBBER	2
#define TYPE_BUBBLEGUM		3
#define TYPE_SPEEDBOOST		4



class CVector3 {
public:
	float X, Y, Z;

	CVector3();
	~CVector3();
	CVector3(const CVector3 & copy);
	CVector3(const float x, const float y, const float z);
	CVector3 Normalize(const CVector3 vVector);
	CVector3 operator + (const CVector3 vVector);
	CVector3 operator - (const CVector3 vVector);
	CVector3 operator * (const float Scalar);
	CVector3 operator / (const float Scalar);
};

float Magnitude(CVector3 vVector);
CVector3 Normalize(CVector3 vVector);

class c3dObject {

public:

	// Our base object constructor
	c3dObject();

	// All in one constructor
	c3dObject(float positionX, float positionY, float positionZ,
		      float viewX,     float viewY,     float viewZ,
			  float upVectorX, float upVectorY, float upVectorZ,
			  UINT textureID);
	
	// This changes the position, view, and up vector of the object.
	// This is primarily used for initialization
	void PositionObject(float positionX, float positionY, float positionZ,
			 		    float viewX,     float viewY,     float viewZ,
						float upVectorX, float upVectorY, float upVectorZ);

	// This rotates the object's view around the position depending on the values passed in.
	void RotateView(float angle, float X, float Y, float Z);

	// This rolls the object's view depending on the speed.
	void RollObject(float speed);

	// This rolls the object's view around the up view depending on the values passed in.
	void RotateUpView(float angle, float x, float y, float z);

	// This will move the camera forward or backward depending on the speed
	void MoveObject(float speed);

	// The object's position
	CVector3 m_vPosition;					

	// The object's view
	CVector3 m_vView;						

	// The object's up vector
	CVector3 m_vUpVector;		
	
	// The object's strafe vector
	CVector3 m_vStrafe;

	// The object's type
	int m_oType;

	// The object's texture ID
	int m_TextureID;
};

class c3dCamera : public c3dObject
{
public:
	void Look(void);
};

class c3dOrange : public c3dObject
{
public:
	short m_State;
	float m_Radius;
	float m_BubbleRadius;
	float m_RotationX;	
	float m_RollSpeed;
	float m_HeightOffset;
	long  m_TimeToJump;
	long  m_LastJumpTime;
	long  m_TimeToSpeed;
	long  m_LastSpeedTime;
	long  m_TimeToPenalty;
	long  m_LastPenaltyTime;
	long  m_TimeToBubble;
	long  m_LastBubbleTime;
	UINT  m_BubbleQty;
	UINT  m_SpeedBoostQty;
	long  m_Modified;
	int	  m_LastOpponent;
	
	// All in one constructor
	c3dOrange(float positionX, float positionY, float positionZ,
		      float viewX,     float viewY,     float viewZ,
			  float upVectorX, float upVectorY, float upVectorZ,
              UINT textureID, float RotationX, float RollSpeed,
			  float HeightOffset, float Radius);
	void Update(void);
	void Render(void);
};

class c3dBubbleGum : public c3dObject
{
public:
	float m_Rotation;	
	float m_RollSpeed;
	float m_HeightOffset;
	short m_State;
	UINT  m_Score;
	
	// All in one constructor
	c3dBubbleGum() { }
	c3dBubbleGum(float positionX, float positionY, float positionZ,
		         float viewX,     float viewY,     float viewZ,
				 float upVectorX, float upVectorY, float upVectorZ,
				 UINT textureID, float Rotation,
				 float HeightOffset, short State);
	void Update(void);
	void Render(void);
};

class c3dSpeedBoost : public c3dObject
{
public:
	float m_Rotation;	
	float m_RollSpeed;
	float m_HeightOffset;
	short m_State;
	UINT  m_Score;
	
	// All in one constructor
	c3dSpeedBoost() { }
	c3dSpeedBoost(float positionX, float positionY, float positionZ,
		          float viewX,     float viewY,     float viewZ,
				  float upVectorX, float upVectorY, float upVectorZ,
				  UINT textureID, float Rotation,
				  float HeightOffset, short State);
	void Update(void);
	void Render(void);
};

class c3dCandyCorn : public c3dObject
{
public:
	float m_Rotation;	
	float m_RollSpeed;
	float m_HeightOffset;
	float m_BubbleRadius;
	short  m_State;
	UINT  m_Score;

	c3dCandyCorn() { }
	
	// All in one constructor
	c3dCandyCorn(float positionX, float positionY, float positionZ,
		         float viewX,     float viewY,     float viewZ,
				 float upVectorX, float upVectorY, float upVectorZ,
				 UINT textureID, float Rotation,
				 float HeightOffset, short State);
	void Update(void);
	void Render(void);
};


class c3dCandyGobber : public c3dObject
{
public:
	float    m_Rotation;	
	float    m_RollSpeed;
	float    m_HeightOffset;
	float    m_Radius;
	float    m_BubbleRadius;
	short    m_State;
	CVector3 m_Color;
	UINT     m_Score;

	c3dCandyGobber() { }
	
	// All in one constructor
	c3dCandyGobber(float positionX, float positionY, float positionZ,
		           float viewX,     float viewY,     float viewZ,
				   float upVectorX, float upVectorY, float upVectorZ,
				   UINT textureID, float Rotation,
				   float HeightOffset, float Radius, short State);
	void Update(void);
	void Render(void);
};


#endif