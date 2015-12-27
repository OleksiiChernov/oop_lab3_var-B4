// (C) 2013-2015, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#include "employee.hpp"
#include "messages.hpp"
#include <exception>
#include <string>

Employee::Employee(std::string const & _employeeName, double _salaryBonus)

	: m_employeeName(_employeeName), m_salaryBonus(_salaryBonus)
{
	if (_employeeName == "")
		throw std::logic_error(Messages::EmployeeFullNameEmpty);
	
	if (m_salaryBonus < 0)
		throw std::logic_error(Messages::NegativeBonusAmount);
	
}

double Employee::calculateSalaryPerYear(double _baseSalary)
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


void Employee::setManagerName(std::string  _managerName)
{
		m_managerName = _managerName;
}

void Employee::delManager()
{
	m_managerName = "";
}

double Employee::calculateSalaryPerMounth(double _baseSalary)
{
	if (_baseSalary < 0)
		throw std::logic_error(Messages::NegativeBaseSalary);
	return _baseSalary + (double)m_salaryBonus;
}
