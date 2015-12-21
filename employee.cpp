// (C) 2013-2015, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#include "employee.hpp"
#include "messages.hpp"
#include <exception>


Employee::Employee(std::string const & _employeeName, double _salaryBonus)
	: m_employeeName(_employeeName), m_salaryBonus(_salaryBonus)
{
	if (_employeeName == "")
		throw std::logic_error(Messages::EmployeeFullNameEmpty);
	
	if (m_salaryBonus < 0)
		throw std::logic_error(Messages::NegativeBonusAmount);
	
}

float Employee::calculateSalaryPerYear(float _baseSalary)
{
	return 12 * calculateSalaryPerMounth(_baseSalary);
}

void Employee::setSalaryBonus(double _bonus)
{
	if (_bonus < 0)
		throw std::logic_error(Messages::NegativeBonusAmount);
	else
		m_salaryBonus = _bonus;
}

float Employee::calculateSalaryPerMounth(float _baseSalary)
{
	if (_baseSalary < 0)
		throw std::logic_error(Messages::NegativeBaseSalary);
	return _baseSalary + (float)m_salaryBonus;
}
