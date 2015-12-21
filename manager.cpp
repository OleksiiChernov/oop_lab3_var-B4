// (C) 2013-2015, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#include "manager.hpp"
#include "messages.hpp"


float Manager::getManagerSalary(float _baseSalary)
{
	float sum = 0;

	for (Employee * _empl : m_subordinateEmployee)
		sum += _empl->calculateSalaryPerMounth(_baseSalary);

	return 2 * sum + getSalaryBonus();
}
