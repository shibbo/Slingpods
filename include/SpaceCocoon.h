#pragma once

#include "Game/LiveActor/LiveActor.h"
#include "JSystem/JGeometry/TMatrix.h"
#include "Game/LiveActor/HitSensor.h"
#include "Game/Util/Color8.h"

class ActorCameraInfo;
class CameraTargetMtx;
class PlantPoint;
class PartsModel;
class JUTTexture;

class SpaceCocoon : public LiveActor {
public:
    SpaceCocoon(const char *);

    virtual ~SpaceCocoon();
    virtual void init(const JMapInfoIter &);
    virtual void draw() const;
    virtual void calcAnim();
    virtual void attackSensor(HitSensor *, HitSensor *);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor *, HitSensor *);
    virtual bool recieveOtherMsg(u32, HitSensor *,  HitSensor *);

    void exeStop();
    void exeFree();
    void exeFreeInvalid();
    void exeBindLand();
    void exeBindWait();
    void exeWaitKinopioAimDemo();
    void exeBindAim();
    void exeBindAttack();
    void exeBindAttackSuccess();
    bool updateBindWait();
    bool updateSpringPoint();
    void updateHang();
    void updateBindAttack();
    void updateActorMtx();
    void updateDrawPoints();
    bool tryTouch();
    bool tryRelease();
    bool tryAttackMap();
    bool isKinopioAttached() const;
    void endBind(const TVec3f &, bool);
    void endCommandStream();
    void drawPlane(f32, f32, f32, f32, Color8, Color8, f32, f32);
    void initDraw() const;

    bool _8C;       // obj_arg3
    TVec3f _90;
    TVec3f _9C;
    TVec3f _A8;
    TVec3f _B4;
    TVec3f _C0;
    f32 _CC;        // obj_arg1
    TVec3f _D0;
    s32 _DC;
    TVec3f _E0;
    TVec3f _EC;
    s32 _F8;        // obj_arg0
    u32 _FC;
    LiveActor* _100;
    TPos3f _104;
    u32 _134;
    PlantPoint** mPlantPoints;          // 0x138
    CameraTargetMtx* mTargetMtx;        // 0x13C
    ActorCameraInfo* mCameraInf;        // 0x140
    PartsModel* mCocoonModel;           // 0x144
    TPos3f _148;
    JUTTexture* mTex;                   // 0x178
};