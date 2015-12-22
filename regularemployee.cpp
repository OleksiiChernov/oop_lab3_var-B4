// (C) 2013-2015, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#include "regularemployee.hpp"
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



float RegularEmployee::calculateSalaryPerMounth(float _baseSalary)
{
	switch (m_employeeLevel)
	{
		case RegularEmployeeLevel::Junior:
		{
			return calculateSalaryPerMounth(0.5 * _baseSalary);
			break;
		}
		case RegularEmployeeLevel::Middle:
		{
			return calculateSalaryPerMounth( _baseSalary);
			break;
		}
		case RegularEmployeeLevel::Senior:
		{
			return calculateSalaryPerMounth(1.2 * _baseSalary);
			break;
		}
		case RegularEmployeeLevel::Lead:
		{
			return calculateSalaryPerMounth(1.5 * _baseSalary);
			break;
		}
	}
}



float RegularEmployee::calculateSalaryPerYear(float _baseSalary)
{
	switch (m_employeeLevel)
	{
		case RegularEmployeeLevel::Junior:
		{
			return calculateSalaryPerYear(0.5 * _baseSalary);
			break;
		}
		case RegularEmployeeLevel::Middle:
		{
			return calculateSalaryPerYear(_baseSalary);
			break;
		}
		case RegularEmployeeLevel::Senior:
		{
			return calculateSalaryPerYear(1.2 * _baseSalary);
			break;
		}
		case RegularEmployeeLevel::Lead:
		{
			return calculateSalaryPerYear(1.5 * _baseSalary);
			break;
		}
	}
}
