#pragma once

#include "Eternal/Core/Core.h"
#include "Layer.h"
#include <vector>

namespace Eternal {

	class ETERNAL_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(SharedPtr<Layer> layer);
		void PushOverlay(SharedPtr<Layer> overlay);
		void PopLayer(SharedPtr<Layer> layer);
		void PopOverlay(SharedPtr<Layer> overlay);

		std::vector<SharedPtr<Layer>>::iterator begin() { return m_Layers.begin(); }
		std::vector<SharedPtr<Layer>>::iterator end() { return m_Layers.end(); }
		std::vector<SharedPtr<Layer>>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		std::vector<SharedPtr<Layer>>::reverse_iterator rend() { return m_Layers.rend(); }
	
		std::vector<SharedPtr<Layer>>::const_iterator begin() const { return m_Layers.begin(); }
		std::vector<SharedPtr<Layer>>::const_iterator end()	const { return m_Layers.end(); }
		std::vector<SharedPtr<Layer>>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
		std::vector<SharedPtr<Layer>>::const_reverse_iterator rend() const { return m_Layers.rend(); }

	private:
		std::vector<SharedPtr<Layer>> m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};
}