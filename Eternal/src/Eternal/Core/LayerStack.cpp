#include "ETpch.h"
#include "LayerStack.h"

namespace Eternal {

	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
		//for ( auto it = m_Layers.begin(); it < m_Layers.end(); ++it)
		//{
		//	m_Layers.erase(it);
		//}

	}

	void LayerStack::PushLayer(SharedPtr<Layer> layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
	}

	void LayerStack::PushOverlay(SharedPtr<Layer> overlay)
	{
		m_Layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(SharedPtr<Layer> layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end()) 
		{
			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(SharedPtr<Layer> overlay)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		if (it != m_Layers.end())
			m_Layers.erase(it);
	}
}