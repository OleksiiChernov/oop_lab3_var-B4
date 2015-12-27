// (C) 2013-2015, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#include "regularemployee.hpp"
#include "messages.hpp"
#include <string>


RegularEmployee::RegularEmployee(std::string const & _employeeName, RegularEmployeeLevel _lvl, double _salaryBonus )
	: Employee(_employeeName, _salaryBonus), m_employeeLevel(_lvl)
{ }


void RegularEmployee::levelUp()
{
	if ((int)m_employeeLevel < 3)
	{
		int a = (int)m_employeeLevel;
		a++;
		m_employeeLevel = (RegularEmployeeLevel)a;
	}
}



double RegularEmployee::calculateSalaryPerMounth(double _baseSalary)
{
	if (_baseSalary < 0)
		throw std::logic_error(Messages::NegativeBaseSalary);

	switch (m_employeeLevel)
	{
		case RegularEmployeeLevel::Junior:
		{
			return 0.5 * _baseSalary + getSalaryBonus();
			break;
		}
		case RegularEmployeeLevel::Middle:
		{
			return  _baseSalary + getSalaryBonus();
			break;
		}
		case RegularEmployeeLevel::Senior:
		{
			return 1.2 * _baseSalary + getSalaryBonus();
			break;
		}
		case RegularEmployeeLevel::Lead:
		{
			return 1.5 * _baseSalary + getSalaryBonus();
			break;
		}
	}
}



double RegularEmployee::calculateSalaryPerYear(double _baseSalary)
{
	switch (m_employeeLevel)
	{
		case RegularEmployeeLevel::Junior:
		{
			return 12 * calculateSalaryPerMounth(0.5 * _baseSalary);
			break;
		}
		case RegularEmployeeLevel::Middle:
		{
			return 12 * calculateSalaryPerMounth(_baseSalary);
			break;
		}
		case RegularEmployeeLevel::Senior:
		{
			return 12 * calculateSalaryPerMounth(1.2 * _baseSalary);
			break;
		}
		case RegularEmployeeLevel::Lead:
		{
			return 12 * calculateSalaryPerMounth(1.5 * _baseSalary);
			break;
		}
	}
}
