#ifndef TPOVIEWCAMERA_H
#define TPOVIEWCAMERA_H


namespace drawlib
{
class CameraUniversal;
}


class MtAdapterOption;
class QMouseEvent;
class QWheelEvent;
class QKeyEvent;


class TpoViewCamera
{
    friend class TpoView;
    TpoViewCamera( MtAdapterOption *adapterOption );
    
    MtAdapterOption *m_adapterOption;
    drawlib::CameraUniversal *m_cameraUniversal;
    int m_cameraMouseOldX;
    int m_cameraMouseOldY;
    
    void p_updateCameraToOptions();
    
public:
    virtual ~TpoViewCamera();
    
    void updateOptionsToCamera();
    
    bool mousePressEvent( QMouseEvent* e , int typeCamera );
    bool mouseReleaseEvent( QMouseEvent* e , int typeCamera );
    bool mouseMoveEvent( QMouseEvent* e, int typeCamera, bool flag_updateOld );
    
    bool wheelEvent( QWheelEvent * e );
    
    void keyPressEvent( QKeyEvent *e );
    bool keyReleaseEvent( QKeyEvent *e );
    
    float * getMtxView();
    float * getMtxProjection();
};


#endif
