#include "tpoViewCamera.h"
#include <qevent.h>
#include "enum_struct.h"
#include "mt_adapterOption.h"
#include <math.h>
#include "drawlib/helpers/camera_universal.h"


TpoViewCamera::TpoViewCamera( MtAdapterOption *adapterOption )
{
    m_adapterOption = adapterOption;
    
    m_cameraUniversal = new drawlib::CameraUniversal;
    m_cameraMouseOldX = 0;
    m_cameraMouseOldY = 0;
}


TpoViewCamera::~TpoViewCamera()
{
    delete m_cameraUniversal;
}



/*********************************************************************************
 ********************************** private **************************************
 ********************************************************************************/

void TpoViewCamera::p_updateCameraToOptions()
{
    // action* func in camera may change only position and target
    // anothe option set only from "updateOptionsToCamera" function
    
    m_adapterOption->cameraPosX() = m_cameraUniversal->getPositionX();
    m_adapterOption->cameraPosY() = m_cameraUniversal->getPositionY();
    m_adapterOption->cameraPosZ() = m_cameraUniversal->getPositionZ();
    
    m_adapterOption->cameraTargetX() = m_cameraUniversal->getTargetX();
    m_adapterOption->cameraTargetY() = m_cameraUniversal->getTargetY();
    m_adapterOption->cameraTargetZ() = m_cameraUniversal->getTargetZ();
}



/*********************************************************************************
 *********************************** public **************************************
 ********************************************************************************/

void TpoViewCamera::updateOptionsToCamera()
{
    // all option for camera
    // m_cameraAngle, m_cameraScale - use in key/mouse prepare
    
    m_cameraUniversal->setPositionXYZ(
        m_adapterOption->cameraPosX(),
        m_adapterOption->cameraPosY(),
        m_adapterOption->cameraPosZ() );
        
    m_cameraUniversal->setTarget(
        m_adapterOption->cameraTargetX(),
        m_adapterOption->cameraTargetY(),
        m_adapterOption->cameraTargetZ() );
        
    m_cameraUniversal->setProjectionDegres( m_adapterOption->cameraDegres() );
    m_cameraUniversal->setProjectionProportion( m_adapterOption->cameraProportion() );
    m_cameraUniversal->setProjectionNear( m_adapterOption->cameraNear() );
    m_cameraUniversal->setProjectionFar( m_adapterOption->cameraFar() );
    
    m_cameraUniversal->setCameraSpeed( m_adapterOption->cameraMove() );
}


bool TpoViewCamera::mousePressEvent( QMouseEvent* e, int typeCamera )
{
    if( ( e->button() & Qt::MidButton ) ||
        ( e->button() & Qt::RightButton ) ||
        typeCamera != TYPE_CAMERA_SELECTOR )
    {
        m_cameraMouseOldX = e->x();
        m_cameraMouseOldY = e->y();
        return true;
    }
    
    return false;
}


bool TpoViewCamera::mouseReleaseEvent( QMouseEvent* e, int typeCamera )
{
    if( ( e->button() & Qt::MidButton ) ||
        ( e->button() & Qt::RightButton ) ||
        typeCamera != TYPE_CAMERA_SELECTOR )
    {
        p_updateCameraToOptions();
        return true;
    }
    
    return false;
}


bool TpoViewCamera::mouseMoveEvent( QMouseEvent* e, int typeCamera, bool flag_updateOld )
{
    if( ( e->state() & Qt::MidButton ) ||
        ( ( e->state() & Qt::LeftButton ) && typeCamera == TYPE_CAMERA_HAND ) )
    {
        float oldSpeed = m_cameraUniversal->getCameraSpeed();
        m_cameraUniversal->setCameraSpeed( oldSpeed / 10.0f );
        
        for( int i = 0; i < fabs( e->x() - m_cameraMouseOldX ); i++ )
        {
            if( ( e->x() - m_cameraMouseOldX ) > 0 )
                m_cameraUniversal->actionAsideForvard();
            else
                m_cameraUniversal->actionAsideReverse();
        }
        
        for( int i = 0; i < fabs( e->y() - m_cameraMouseOldY ); i++ )
        {
            if( ( e->y() - m_cameraMouseOldY ) > 0 )
                m_cameraUniversal->actionUpVectorForvard();
            else
                m_cameraUniversal->actionUpVectorReverse();
        }
        
        m_cameraUniversal->setCameraSpeed( oldSpeed );
    }
    else if( ( e->state() & Qt::RightButton ) ||
             ( ( e->state() & Qt::LeftButton ) && ( typeCamera == TYPE_CAMERA_ORBIT || typeCamera == TYPE_CAMERA_FPS ) ) )
    {
        float stepAngle = m_adapterOption->cameraAngle() / 25.0f;
        float resRotateX = m_cameraUniversal->getRotationX();
        float resRotateY = m_cameraUniversal->getRotationY();
        resRotateY -= ( float )( e->x() - m_cameraMouseOldX ) * stepAngle;
        resRotateX -= ( float )( e->y() - m_cameraMouseOldY ) * stepAngle;
        
        if( ( e->state() & Qt::RightButton ) ||
            ( ( e->state() & Qt::LeftButton ) && typeCamera == TYPE_CAMERA_ORBIT ) )
            m_cameraUniversal->setRotationOrbit( resRotateX, resRotateY );
        else
            m_cameraUniversal->setRotationXY( resRotateX, resRotateY );
    }
    else return false;
    
    if( flag_updateOld )
    {
        m_cameraMouseOldX = e->x();
        m_cameraMouseOldY = e->y();
    }
    
    p_updateCameraToOptions();
    
    return true;
}


bool TpoViewCamera::wheelEvent( QWheelEvent * e )
{
    if( ( e->state() & Qt::MidButton ) ) return false;
    
    float oldSpeed = m_cameraUniversal->getCameraSpeed();
    m_cameraUniversal->setCameraSpeed( m_adapterOption->cameraScale() );
    ( e->delta() < 0 ) ? m_cameraUniversal->actionDirectionForvard() : m_cameraUniversal->actionDirectionReverse();
    m_cameraUniversal->setCameraSpeed( oldSpeed );
    
    p_updateCameraToOptions();
    
    return true;
}


void TpoViewCamera::keyPressEvent( QKeyEvent *e )
{
    if( e->state() & Qt::ShiftButton )
    {
        float stepAngle = m_adapterOption->cameraAngle();
        float resRotateX = m_cameraUniversal->getRotationX();
        float resRotateY = m_cameraUniversal->getRotationY();
        
        if( e->key() == Qt::Key_Down )
            resRotateX += stepAngle;
            
        if( e->key() == Qt::Key_Up )
            resRotateX -= stepAngle;
            
        if( e->key() == Qt::Key_Left )
            resRotateY += stepAngle;
            
        if( e->key() == Qt::Key_Right )
            resRotateY -= stepAngle;
            
        m_cameraUniversal->setRotationOrbit( resRotateX, resRotateY );
    }
    else
    {
        if( e->key() == Qt::Key_Down )
            m_cameraUniversal->actionForvardForvard();
            
        if( e->key() == Qt::Key_Up )
            m_cameraUniversal->actionForvardReverse();
            
        if( e->key() == Qt::Key_Left )
            m_cameraUniversal->actionAsideForvard();
            
        if( e->key() == Qt::Key_Right )
            m_cameraUniversal->actionAsideReverse();
            
        if( e->key() == Qt::Key_Home )
            m_cameraUniversal->actionVerticalForvard();
            
        if( e->key() == Qt::Key_End )
            m_cameraUniversal->actionVerticalReverse();
            
        if( e->key() == Qt::Key_PageUp )
        {
            float oldSpeed = m_cameraUniversal->getCameraSpeed();
            m_cameraUniversal->setCameraSpeed( m_adapterOption->cameraScale() );
            m_cameraUniversal->actionDirectionReverse();
            m_cameraUniversal->setCameraSpeed( oldSpeed );
        }
        
        if( e->key() == Qt::Key_PageDown )
        {
            float oldSpeed = m_cameraUniversal->getCameraSpeed();
            m_cameraUniversal->setCameraSpeed( m_adapterOption->cameraScale() );
            m_cameraUniversal->actionDirectionForvard();
            m_cameraUniversal->setCameraSpeed( oldSpeed );
        }
    }
}


bool TpoViewCamera::keyReleaseEvent( QKeyEvent *e )
{
    if( e->key() == Qt::Key_Left ||
        e->key() == Qt::Key_Right ||
        e->key() == Qt::Key_Up ||
        e->key() == Qt::Key_Down ||
        e->key() == Qt::Key_PageUp ||
        e->key() == Qt::Key_PageDown ||
        e->key() == Qt::Key_Home ||
        e->key() == Qt::Key_End )
    {
        p_updateCameraToOptions();
        return true;
    }
    
    return false;
}


float * TpoViewCamera::getMtxView()
{
    return m_cameraUniversal->getMtxView();
}


float * TpoViewCamera::getMtxProjection()
{
    return m_cameraUniversal->getMtxProjection();
}
