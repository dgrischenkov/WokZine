#include "drawlib/helpers/camera_universal.h"
#include "drawlib/math/matrix_dl.h"
#include "drawlib/math/vec3_dl.h"

#define TO_RADIAN(var)  var*float(0.0174532)
#define TO_DEGREES(var) var*float(57.295779)
#define GLHDL_EPSILON   double(10e-6)


namespace drawlib
{


CameraUniversal::CameraUniversal()
{
    m_targetX = m_targetY = m_targetZ = 0.0f;
    m_posX = m_posY = m_posZ = 0.0f;
    m_rotX = m_rotY = 0.0f;
    
    m_projectionDegres = 90.0f;
    m_projectionProportion = 1.0f;
    m_projectionNear = 0.001f;
    m_projectionFar = 10.0f;
    
    m_cameraSpeed = 0.01f;
    
    p_updateView();
    p_updateProjection();
}


CameraUniversal::~CameraUniversal()
{
}



/************************************************************************
 ******************************* private ********************************
 ***********************************************************************/

void CameraUniversal::p_actionProcess( int action )
{
    if( action == ECUNI_NONE ) return;
    
    vec3DL direction;
    
    if( action == ECUNI_VERTICAL_FORVARD ||
        action == ECUNI_VERTICAL_REVERSE )
    {
        direction[0] = 0.0f;
        direction[1] = 0.0f;
        direction[2] = 1.0f;
    }
    else
    {
        matrixDL dirMatrix;
        dirMatrix.set_angle( 0.0f, 0.0f, TO_RADIAN( m_rotY ) );
        dirMatrix.set_angle_append( TO_RADIAN( m_rotX ), 0.0f, 0.0f );
        dirMatrix = dirMatrix.inverse();
        direction[0] =  dirMatrix[8];
        direction[1] = -dirMatrix[9];
        direction[2] =  dirMatrix[10];
        
        vec3DL directionForward = direction;
        
        if( action != ECUNI_DIRECTION_FORVARD &&
            action != ECUNI_DIRECTION_REVERSE )
        {
            float projectionX = direction.dot( vec3DL( 1.0f, 0.0f, 0.0f ) );
            float projectionY = direction.dot( vec3DL( 0.0f, 1.0f, 0.0f ) );
            direction.reset( projectionX, projectionY, 0.0f );
            direction.normalize();
            
            if( action == ECUNI_ASIDE_FORVARD ||
                action == ECUNI_ASIDE_REVERSE ||
                action == ECUNI_UPVECTOR_FORVARD ||
                action == ECUNI_UPVECTOR_REVERSE )
            {
                direction[2] =  direction[1];
                direction[1] = -direction[0];
                direction[0] =  direction[2];
                direction[2] =  0.0f;
                
                if( action == ECUNI_UPVECTOR_FORVARD ||
                    action == ECUNI_UPVECTOR_REVERSE )
                {
                    direction = direction.cross( directionForward );
                }
            }
        }
    }
    
    if( action == ECUNI_DIRECTION_REVERSE ||
        action == ECUNI_FORVARD_REVERSE ||
        action == ECUNI_ASIDE_REVERSE ||
        action == ECUNI_VERTICAL_REVERSE ||
        action == ECUNI_UPVECTOR_REVERSE ) direction *= -1.0f;
        
    direction *= m_cameraSpeed;
    
    
    if( action != ECUNI_DIRECTION_FORVARD &&
        action != ECUNI_DIRECTION_REVERSE )
    {
        m_targetX += direction[0];
        m_targetY += direction[1];
        m_targetZ += direction[2];
    }
    else if( action == ECUNI_DIRECTION_REVERSE )
    {
        vec3DL vecPlusDir( direction[0], direction[1], direction[2] );
        vec3DL vecTarget( m_targetX, m_targetY, m_targetZ );
        vec3DL vecPosCam( m_posX, m_posY, m_posZ );
        
        float len0 = ( vecPosCam + vecPlusDir - vecTarget ).length();
        float len1 = ( vecPosCam - vecTarget ).length();
        
        if( len0 > len1 )
        {
            action = ECUNI_NONE;
            return;
        }
    }
    
    
    m_posX += direction[0];
    m_posY += direction[1];
    m_posZ += direction[2];
    
    action = ECUNI_NONE;
}


void CameraUniversal::p_updateView()
{
    matrixDL viewMatrix;
    viewMatrix.set_angle( TO_RADIAN( m_rotX ), 0.0f, 0.0f );
    viewMatrix.set_angle_append( 0.0f, 0.0f, TO_RADIAN( m_rotY ) );
    viewMatrix.set_translate( vec3DL( m_posX, m_posY, m_posZ ) );
    viewMatrix = viewMatrix.inverse();
    
    // convert double to float
    for( int i = 0; i < 16; i++ ) m_outMtxView[i] = viewMatrix[i];
}


void CameraUniversal::p_updateProjection()
{
    matrixDL mtxMtx;
    mtxMtx.set_perspective(
        TO_RADIAN( m_projectionDegres ),
        m_projectionProportion,
        m_projectionNear,
        m_projectionFar );
        
    // convert double to float
    for( int i = 0; i < 16; i++ ) m_outMtxProjection[i] = mtxMtx[i];
}



/************************************************************************
 **************************** public, action ****************************
 ***********************************************************************/

void CameraUniversal::actionDirectionForvard()
{
    p_actionProcess( ECUNI_DIRECTION_FORVARD );
}


void CameraUniversal::actionForvardForvard()
{
    p_actionProcess( ECUNI_FORVARD_FORVARD );
}


void CameraUniversal::actionAsideForvard()
{
    p_actionProcess( ECUNI_ASIDE_FORVARD );
}


void CameraUniversal::actionVerticalForvard()
{
    p_actionProcess( ECUNI_VERTICAL_FORVARD );
}


void CameraUniversal::actionUpVectorForvard()
{
    p_actionProcess( ECUNI_UPVECTOR_FORVARD );
}


void CameraUniversal::actionDirectionReverse()
{
    p_actionProcess( ECUNI_DIRECTION_REVERSE );
}


void CameraUniversal::actionForvardReverse()
{
    p_actionProcess( ECUNI_FORVARD_REVERSE );
}


void CameraUniversal::actionAsideReverse()
{
    p_actionProcess( ECUNI_ASIDE_REVERSE );
}


void CameraUniversal::actionVerticalReverse()
{
    p_actionProcess( ECUNI_VERTICAL_REVERSE );
}


void CameraUniversal::actionUpVectorReverse()
{
    p_actionProcess( ECUNI_UPVECTOR_REVERSE );
}



/************************************************************************
 **************************** public matrix *****************************
 ***********************************************************************/

float *CameraUniversal::getMtxView()
{
    p_updateView();
    return m_outMtxView;
}


float *CameraUniversal::getMtxProjection()
{
    p_updateProjection();
    return m_outMtxProjection;
}



/************************************************************************
 *************************** public position ****************************
 ***********************************************************************/

void CameraUniversal::setPositionXYZ( float posX, float posY, float posZ )
{
    m_targetX += m_posX - posX;
    m_targetY += m_posY - posY;
    m_targetZ += m_posZ - posZ;
    
    m_posX = posX; m_posY = posY; m_posZ = posZ;
}


float CameraUniversal::getPositionX() const
{
    return m_posX;
}


float CameraUniversal::getPositionY() const
{
    return m_posY;
}


float CameraUniversal::getPositionZ() const
{
    return m_posZ;
}



/************************************************************************
 *************************** public rotation ****************************
 ***********************************************************************/

void CameraUniversal::setRotationOrbit( float rotXOrbit, float rotYOrbit )
{
    if( rotXOrbit > 179.0f ) rotXOrbit = 179.0f;
    if( rotXOrbit <   1.0f ) rotXOrbit =   1.0f;
    
    double R = len( m_posX, m_posY, m_posZ, m_targetX, m_targetY, m_targetZ );
    
    double f = TO_RADIAN( rotYOrbit );
    double t = TO_RADIAN( rotXOrbit );
    
    m_posX = sin( t ) * sin( f ) *  R + m_targetX;
    m_posY = sin( t ) * cos( f ) * -R + m_targetY;
    m_posZ = cos( t ) * R           + m_targetZ;
    
    m_rotX = rotXOrbit;
    m_rotY = rotYOrbit;
}


void CameraUniversal::setRotationXY( float rotX, float rotY )
{
    if( rotX > 179.0f ) rotX = 179.0f;
    if( rotX <   1.0f ) rotX =   1.0f;
    
    m_rotX = rotX; m_rotY = rotY;
    
    double f = TO_RADIAN( m_rotY );
    double t = TO_RADIAN( m_rotX );
    
    double R = len( m_posX, m_posY, m_posZ, m_targetX, m_targetY, m_targetZ );
    
    m_targetX = m_posX - sin( t ) * sin( f ) *  R;
    m_targetY = m_posY - sin( t ) * cos( f ) * -R;
    m_targetZ = m_posZ - cos( t ) * R;
}


float CameraUniversal::getRotationX() const
{
    return m_rotX;
}


float CameraUniversal::getRotationY() const
{
    return m_rotY;
}



/************************************************************************
 **************************** public target *****************************
 ***********************************************************************/

void CameraUniversal::setTarget( float posXTarget, float posYTarget, float posZTarget )
{
    vec3DL vecFrom( m_posX,     m_posY,     m_posZ );
    vec3DL vecTo( posXTarget, posYTarget, posZTarget );
    vecFrom -= vecTo;
    vecFrom.normalize();
    
    double vecX, vecY, vecZ, vecXpre, vecYpre;
    vecFrom.values( vecX, vecY, vecZ );
    
    float t = acos( vecZ );
    
    if( sin( t ) <  GLHDL_EPSILON &&
        sin( t ) > -GLHDL_EPSILON ) return;
        
    vecXpre =  vecX / sin( t );
    vecYpre = -vecY / sin( t );
    
    if( fabs( vecXpre ) > 1.0 ||
        fabs( vecYpre ) > 1.0 ) return;
        
    float f = acos( vecYpre );
    float u = asin( vecXpre );
    
    m_rotX = TO_DEGREES( t );
    m_rotY = TO_DEGREES( ( ( u < 0 ) ? -f : f ) );
    
    m_targetX = posXTarget;
    m_targetY = posYTarget;
    m_targetZ = posZTarget;
}


float CameraUniversal::getTargetX() const
{
    return m_targetX;
}


float CameraUniversal::getTargetY() const
{
    return m_targetY;
}


float CameraUniversal::getTargetZ() const
{
    return m_targetZ;
}



/************************************************************************
 ************************** public projection ***************************
 ***********************************************************************/

void CameraUniversal::setProjectionDegres( float degres )
{
    m_projectionDegres = degres;
}


void CameraUniversal::setProjectionProportion( float proportion )
{
    m_projectionProportion = proportion;
}


void CameraUniversal::setProjectionNear( float near )
{
    m_projectionNear = near;
}


void CameraUniversal::setProjectionFar( float far )
{
    m_projectionFar = far;
}


float CameraUniversal::getProjectionDegres() const
{
    return m_projectionDegres;
}


float CameraUniversal::getProjectionProportion() const
{
    return m_projectionProportion;
}


float CameraUniversal::getProjectionNear() const
{
    return m_projectionNear;
}


float CameraUniversal::getProjectionFar() const
{
    return m_projectionFar;
}



/************************************************************************
 **************************** public speed ******************************
 ***********************************************************************/

void CameraUniversal::setCameraSpeed( float cameraSpeed )
{
    m_cameraSpeed = cameraSpeed;
}


float CameraUniversal::getCameraSpeed() const
{
    return m_cameraSpeed;
}


}
