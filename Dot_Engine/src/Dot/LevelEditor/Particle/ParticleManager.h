#pragma once
#include "ParticleSystem.h"

namespace Dot {


	class ParticleManager
	{
	public:
		ParticleManager(unsigned int count)
		{

			std::vector<ParticlePosition>m_pos;
			std::vector<ParticleVelocity>m_vel;

			m_pos.resize(count);
			m_vel.resize(count);

			srand(count);
			for (int i = 0; i < count; ++i)
			{
				m_pos[i].x = 0;
				m_pos[i].y = 0;
				m_pos[i].z = 0;
				m_pos[i].w = 1.0f;


				m_vel[i].vx = -2.5f + rand() % 5 + (rand() % 10) / 100.0f;
				m_vel[i].vy = -2.5f + rand() % 5 + (rand() % 10) / 100.0f;
				m_vel[i].vz = -5.0f + rand() % 10 + (rand() % 10) / 100.0f;
				m_vel[i].fTimeToLive = 5;

			}

			BufferLayout buflayout = {
			{0, ShaderDataType::Float4, "position"}
			};
			
			
			ParticleLayout layout = {
				count,{},
			{
				{"pos",0,2,(void*)&m_pos[0], count * sizeof(ParticlePosition),buflayout,true},
				{"vel",1,3,(void*)&m_vel[0], count * sizeof(ParticleVelocity),buflayout,false}
				
			},
			};

			m_system = std::make_shared<ParticleSystem>(layout);
		}

		void Run()
		{
			m_system->Update(1);
		}

		void Draw()
		{
			m_system->Draw();
		}

	private:
		Ref<ParticleSystem> m_system;

	};

}
