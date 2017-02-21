#ifndef DRAWLIB_CAMERA_UNIVERSAL_H
#define DRAWLIB_CAMERA_UNIVERSAL_H


namespace drawlib
{


class CameraUniversal
{
    enum ECUNI
    {
        ECUNI_NONE = 0,
        ECUNI_DIRECTION_FORVARD,
        ECUNI_DIRECTION_REVERSE,
        ECUNI_FORVARD_FORVARD,
        ECUNI_FORVARD_REVERSE,
        ECUNI_ASIDE_FORVARD,
        ECUNI_ASIDE_REVERSE,
        ECUNI_VERTICAL_FORVARD,
        ECUNI_VERTICAL_REVERSE,
        ECUNI_UPVECTOR_FORVARD,
        ECUNI_UPVECTOR_REVERSE
    };
    
    float m_outMtxView[16];
    float m_outMtxProjection[16];
    
    float m_targetX, m_targetY, m_targetZ;
    float m_posX, m_posY, m_posZ;
    float m_rotX, m_rotY;
    
    float m_projectionDegres;
    float m_projectionProportion;
    float m_projectionNear;
    float m_projectionFar;
    
    float m_cameraSpeed;
    
    // private func
    void p_actionProcess( int action );
    void p_updateView();
    void p_updateProjection();
    
public:
    CameraUniversal();
    virtual ~CameraUniversal();
    
    // action
    void actionDirectionForvard();
    void actionForvardForvard();
    void actionAsideForvard();
    void actionVerticalForvard();
    void actionUpVectorForvard();
    
    void actionDirectionReverse();
    void actionForvardReverse();
    void actionAsideReverse();
    void actionVerticalReverse();
    void actionUpVectorReverse();
    
    // matrix
    float *getMtxView();
    float *getMtxProjection();
    
    // position
    void setPositionXYZ( float posX, float posY, float posZ );
    float getPositionX() const;
    float getPositionY() const;
    float getPositionZ() const;
    
    // rotation
    void setRotationOrbit( float rotXOrbit, float rotYOrbit );
    void setRotationXY( float rotX, float rotY );
    float getRotationX() const;
    float getRotationY() const;
    
    // target
    void setTarget( float posXTarget, float posYTarget, float posZTarget );
    float getTargetX() const;
    float getTargetY() const;
    float getTargetZ() const;
    
    // projection
    void setProjectionDegres( float degres );
    void setProjectionProportion( float proportion );
    void setProjectionNear( float near );
    void setProjectionFar( float far );
    
    float getProjectionDegres() const;
    float getProjectionProportion() const;
    float getProjectionNear() const;
    float getProjectionFar() const;
    
    // speed
    void setCameraSpeed( float cameraSpeed );
    float getCameraSpeed() const;
};


}


#endif
