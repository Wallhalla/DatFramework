#pragma once

/*

This file provides helpers for templating

*/

namespace Template
{
	/**
	 * Struct which checks if the given classes derives from one another
	 */
	template<class Child, class Base>
	struct Derived_from
	{
		static void constraints(Child* p)
		{
			Base* pb = p;
		}

		Derived_from()
		{
			void(*p)(Child*) = constraints;
		}
	};

}

// Global calls

// Checks if Child derives from Base, will produce compilererror
template<class Child, class Base>
void CheckInheritance()
{
	Template::Derived_from<Child, Base>();
	
};