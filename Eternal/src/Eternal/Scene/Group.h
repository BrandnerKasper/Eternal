#pragma once

#include "ETpch.h"

namespace Eternal {
	
	struct Group
	{
		Group() = default;
		Group(std::string name) : m_Name(name), m_ID(++g_ID) {}
		Group(std::string name, int id) : m_Name(name), m_ID(id)
		{
			++g_ID;
		}
		std::string m_Name = "";
		int m_ID = 0;
		static inline uint32_t g_ID = 0;

		bool operator==(const Group& other) const
		{
			return m_ID == other.m_ID;
		}
	};

}