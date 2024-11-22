#pragma once
#include <functional>
#include <list>

namespace ftl
{
	template <typename T>
	class PriorityQueue
	{
	public:
		PriorityQueue(std::function<float(const T&)> score_calculator);

		void Enqueue(const T& elem);
		void PopFront();
		const T& PeekFront() const;
		bool IsEmpty() const;

	private:
		std::function<float(const T&)> m_ScoreCalculator;
		std::list<T> m_Elements;
	};


	template<typename T>
	inline PriorityQueue<T>::PriorityQueue(std::function<float(const T&)> score_calculator)
		: m_ScoreCalculator(score_calculator)
	{
	}

	template<typename T>
	inline void PriorityQueue<T>::Enqueue(const T& elem)
	{
		const float new_score = m_ScoreCalculator(elem);

		for (auto it = m_Elements.rbegin(); it != m_Elements.rend(); it++)
		{
			const float existing_score = m_ScoreCalculator(*it);

			if (existing_score > new_score)
			{
				//it++;
				auto fwd_it = it.base();
				m_Elements.insert(fwd_it, elem);
				return;
			}
		}

		m_Elements.push_front(elem);
	}

	template<typename T>
	inline void PriorityQueue<T>::PopFront()
	{
		m_Elements.pop_front();
	}

	template<typename T>
	inline const T& PriorityQueue<T>::PeekFront() const
	{
		return m_Elements.front();
	}

	template<typename T>
	inline bool PriorityQueue<T>::IsEmpty() const
	{
		return m_Elements.empty();
	}
}