#pragma once

#include "utilities/conditions.h"

namespace nemesis
{
	template<typename Type>
	struct CondVar;

	enum CondType
	{
		NONE,
		FOREACH,
		NEW,
		NEW_ORDER,
		CONDITION_START,
		CONDITION,
		CONDITION_DEFAULT,
		MOD_CODE,
	};

	template<typename Type = std::string>
	struct LinkedVar
	{
		bool preCompile;
		bool hasProcess;
		size_t linecount;
		Type raw;
		VecStr lineblocks;
		std::vector<CondVar<Type>> nestedcond;

		LinkedVar() : preCompile(true), hasProcess(false), linecount(0)
		{
		}

		LinkedVar(const Type& _raw) : preCompile(true), hasProcess(false), linecount(0)
		{
			raw = _raw;
		}

		LinkedVar(const Type& _raw, size_t num) : preCompile(true), hasProcess(false), linecount(num)
		{
			raw = _raw;
		}

		LinkedVar(const Type& _raw, bool process, size_t num) : preCompile(true), hasProcess(process), linecount(num)
		{
			if (process)
			{
				lineblocks.reserve(_raw.length());

				for (auto& ch : _raw)
				{
					lineblocks.push_back(string(1, ch));
				}
			}
			else
			{
				raw = _raw;
			}
		}

		LinkedVar(const CondVar<Type>& cond) : preCompile(true), hasProcess(false), linecount(0)
		{
			nestedcond.push_back(cond);
		}

		LinkedVar(const CondVar<Type>& cond, size_t num) : preCompile(true), hasProcess(false), linecount(num)
		{
			nestedcond.push_back(cond);
		}

		CondVar<Type>& addCond()
		{
			nestedcond.push_back(nemesis::CondVar<Type>());
			return nestedcond.back();
		}

		CondVar<Type>& addCond(const CondType& type)
		{
			nestedcond.push_back(nemesis::CondVar<Type>());
			nestedcond.back().conditionType = type;
			return nestedcond.back();
		}

		CondVar<Type>& addCond(const CondVar<Type>& cond)
		{
			nestedcond.push_back(cond);
			return nestedcond.back();
		}

		CondVar<Type>& addCond(const nemesis::LinkedVar<Type>& nestraw, const std::string& conditions, const CondType& type, size_t linecount = 0)
		{
			nestedcond.push_back(nemesis::CondVar<Type>(nestraw, conditions, type));
			nestedcond.back().rawlist.back().linecount = linecount;
			return nestedcond.back();
		}

		CondVar<Type>& addCond(const nemesis::LinkedVar<Type>& nestraw)
		{
			nestedcond.push_back(nestraw);
			return nestedcond.back();
		}

		CondVar<Type>& backCond()
		{
			return nestedcond.back();
		}

		std::string data()
		{
			if (raw.length() == 0)
			{
				string l;

				for (auto c : lineblocks)
				{
					l.append(c);
				}

				return l;
			}

			return raw;
		}

		std::string compile();

		Type operator=(const Type& _raw)
		{
			return raw = _raw;
		}

		Type operator+(const Type& _raw)
		{
			return raw + _raw;
		}

		bool operator==(const Type& _raw)
		{
			if (preCompile) return data() == _raw;

			string l;

			for (auto c : lineblocks)
			{
				l.append(c);
			}

			return l == _raw;
		}

		bool operator!=(const Type& _raw)
		{
			if (preCompile) return data() != _raw;

			string l;

			for (auto c : lineblocks)
			{
				l.append(c);
			}

			return l != _raw;
		}


	};

	template<typename Type>
	struct CondVar
	{
		CondType conditionType;
		bool isMulti = false;
		bool isOrder = false;
		size_t curnum = 0;
		size_t linenum;
		std::string conditions;
		std::shared_ptr<Condt> n_conditions;
		std::vector<LinkedVar<Type>> rawlist;

		CondVar() {}

		CondVar(std::string _conditions, nemesis::CondType condtype)
		{
			rawlist.push_back(LinkedVar<Type>());
			conditions = _conditions;
			conditionType = condtype;
		}

		CondVar(const LinkedVar<Type> _raw)
		{
			rawlist.push_back(_raw);
		}

		CondVar(const LinkedVar<Type> _raw, std::string _conditions, nemesis::CondType condtype)
		{
			rawlist.push_back(_raw);
			conditions = _conditions;
			conditionType = condtype;
		}

		void UpdateCondition(std::string condition, std::string line);
	};

}
