// (C) 2013-2015, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#include "manager.hpp"
#include "messages.hpp"
#include <string>
#include <algorithm>

float Manager::getManagerSalary(float _baseSalary)
{
	float sum = 0;

	for (Employee * _empl : m_subordinateEmployee)
		sum += _empl->calculateSalaryPerMounth(_baseSalary);

	return 2 * sum + getSalaryBonus();
}

void Manager::setNewEmployee(std::string const & _fullName)
{
	auto it = std::find_if(m_subordinateEmployee.begin(), m_subordinateEmployee.end(), 
		[_fullName](Employee const * _empl)
	{
		if (_empl->getEmployeeName() == _fullName)
			return true;
		else
			return false;
	}
		);
	if (it != m_subordinateEmployee.end())
	{
		Employee * _empl = new Employee(_fullName);
		m_subordinateEmployee.push_back(_empl);
	}
	else
		throw std::logic_error(Messages::SubordinateAlreadyAdded);
}

void Manager::deleteEmployee(std::string const & _fullName)
{
	if(hasSubordinatedEmployee(_fullName))
	{
		auto it = std::find_if(m_subordinateEmployee.begin(), m_subordinateEmployee.end(),
			[_fullName](Employee const * _empl)
		{
			if (_empl->getEmployeeName() == _fullName)
				return true;
			else
				return false;
		}
		);
		int a = (int)*it;
		m_subordinateEmployee.erase(m_subordinateEmployee.begin() + a);
	}
	else
		throw std::logic_error(Messages::UnregisteredEmployeeName);
}

bool Manager::hasSubordinatedEmployee(std::string const & _fullName)
{
	auto it = std::find_if(m_subordinateEmployee.begin(), m_subordinateEmployee.end(),
		[_fullName](Employee const * _empl)
	{
		if (_empl->getEmployeeName() == _fullName)
			return true;
		else
			return false;
	}
	);
	if (it != m_subordinateEmployee.end())
		return true;
	else
		return false;
}


void Manager::fireManager()
{
	for (Employee * _emp : m_subordinateEmployee)
		_emp->delManager();

	m_subordinateEmployee.clear();
}
