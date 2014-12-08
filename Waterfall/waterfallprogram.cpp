#include "waterfallprogram.h"

WaterfallProgram::WaterfallProgram()
    : _lastFrameTP(chrono::system_clock::now())
{
    initSettings();
    initAntTweakBar();
    initParticleSystem();
}

void WaterfallProgram::initParticleSystem()
{
    _particleSystem.setMaxParticlesCount(1000);
    setupParticleSystem();
    _particleSystem.initialize();
}

void WaterfallProgram::initSettings()
{
    _cameraZPosition = 100;
    _cameraFOV = 60.0f;

    _emitterPosition = vec3(0.0f, 0.0f, 0.0f);
    _emitterRadius = vec3(3.0f, 3.0f, 3.0f);
    _minVelocity = vec3(5.0f, 0.0f, 0.0f);
    _velocityRange = vec3(10.0f, 15.f, 0.0f);
    _gravity = vec3(0.0f, -9.8f, 0.0f);

    _minLifeTime = 1;
    _maxLifeTime = 7;
    _minSize = 1;
    _maxSize = 3;

    _particleColor = vec3(0.0f, 0.0f, 1.0f);
    _particleOpacity = 0.4f;
}

void WaterfallProgram::initAntTweakBar()
{
    TwInit(TW_OPENGL, NULL);

    TwBar* bar = TwNewBar("Parameters");
    TwDefine(" Parameters size='500 1000' color='70 100 120' valueswidth=220 iconpos=topleft");

    TwAddVarRW(bar, "Camera Z-position", TW_TYPE_FLOAT,  &_cameraZPosition, "keyincr=s keydecr=w min=10 max=150 step=1");
    TwAddVarRW(bar, "FOV",               TW_TYPE_FLOAT,  &_cameraFOV,       "keyincr=o keydecr=p min=10 max=150 step=5");
    TwAddVarRW(bar, "Rotation",          TW_TYPE_QUAT4F, &_rotation,        "label='Object orientation' opened=true");

    TwStructMember vec3Members[] = {
        { "x", TW_TYPE_FLOAT, offsetof(vec3, x), "step=0.5"},
        { "y", TW_TYPE_FLOAT, offsetof(vec3, y), "step=0.5" },
        { "z", TW_TYPE_FLOAT, offsetof(vec3, z), "step=0.5" }
    };
    TwType vec3Type;
    vec3Type = TwDefineStruct("vec3Type", vec3Members, 3, sizeof(vec3), NULL, NULL);
    TwAddVarRW(bar, "Emitter Position", vec3Type, &_emitterPosition, NULL);
    TwAddVarRW(bar, "Emitter Radius", vec3Type, &_emitterRadius, NULL);
    TwAddVarRW(bar, "Min Velocity", vec3Type, &_minVelocity, NULL);
    TwAddVarRW(bar, "Max Velocity", vec3Type, &_velocityRange, NULL);
    TwAddVarRW(bar, "Gravity", vec3Type, &_gravity, NULL);

    TwAddVarRW(bar, "Min LifeTime", TW_TYPE_FLOAT, &_minLifeTime, "step=0.5");
    TwAddVarRW(bar, "Max LifeTime", TW_TYPE_FLOAT, &_maxLifeTime, "step=0.5");
    TwAddVarRW(bar, "Min Size", TW_TYPE_FLOAT, &_minSize, "step=0.5");
    TwAddVarRW(bar, "Max Size", TW_TYPE_FLOAT, &_maxSize, "step=0.5");

    TwAddVarRW(bar, "Partice Color", TW_TYPE_COLOR3F, &_particleColor, NULL);
    TwAddVarRW(bar, "Partice Opacity", TW_TYPE_FLOAT, &_particleOpacity, NULL);
}

void WaterfallProgram::setupParticleSystem()
{
    _particleSystem.setEmitterPosition(_emitterPosition);
    _particleSystem.setEmitterRadius(_emitterRadius);
    _particleSystem.setMinVelocity(_minVelocity);
    _particleSystem.setVelocityRange(_velocityRange);
    _particleSystem.setGravity(_gravity);
    _particleSystem.setMinLifeTime(_minLifeTime);
    _particleSystem.setMaxLifeTime(_maxLifeTime);
    _particleSystem.setMinSize(_minSize);
    _particleSystem.setMaxSize(_maxSize);
    _particleSystem.setInitColor(_particleColor);
    _particleSystem.setInitOpacity(_particleOpacity);
}

void WaterfallProgram::drawFrame()
{
    setupParticleSystem();
    float timePassed = updateTimer();
    _particleSystem.updateParticles(timePassed);

    float const width = (float)glutGet(GLUT_WINDOW_WIDTH);
    float const height = (float)glutGet(GLUT_WINDOW_HEIGHT);
    mat4 mProj = perspective(_cameraFOV, width / height, 0.1f, 150.0f);

    vec3 cameraPosition = vec3(0, 0, _cameraZPosition);
    vec3 viewCenter = vec3(0, 0, 0);
    vec3 upVector = vec3(0, 1, 0);

    _particleSystem.setMatrices(mProj, cameraPosition, viewCenter, upVector, _rotation);
    _particleSystem.renderParticles();
}

float WaterfallProgram::updateTimer()
{
    chrono::system_clock::time_point now = chrono::system_clock::now();
    float timePassed = chrono::duration<float>(now - _lastFrameTP).count();
    _lastFrameTP = now;

    return timePassed;
}