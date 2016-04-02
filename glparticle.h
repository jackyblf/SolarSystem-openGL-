#include "base.h"
class glParticleSystem;

struct glParticle
{
	vec3    m_pos;
	vec3    m_prevPos;
	vec3    m_velocity;
	vec3    m_acceleration;
	float   m_energy;

	float   m_size;
	float   m_sizeDelta;

	float   m_weight;
	float   m_weightDelta;

	float   m_color[4];
	float   m_colorDelta[4];
};


class glParticleSystem
{
public:

	glParticleSystem(int maxParticles, vec3 origin);

	virtual ~glParticleSystem() { KillSystem(); }

	virtual void  Update(float elapsedTime) = 0;
	virtual void  Render() = 0;

	virtual int   Emit(int numParticles);

	virtual void  InitializeSystem();
	virtual void  KillSystem();

protected:
	//纯虚函数，子类override
	virtual void  InitializeParticle(int index) = 0;

	//指针指向glParticle数组首地址
	//数量 = m_maxParticles
	//由于粒子系统会产生大量的小对象，因此用预先分配内存方式提高效率
	glParticle    *m_particleList;

	//最多可以产生的粒子数量
	int            m_maxParticles;

	//当前的粒子数量
	int            m_numParticles;

	//粒子发生的位置坐标
	vec3           m_origin;

	float          m_accumulatedTime;

	vec3           m_force;
};