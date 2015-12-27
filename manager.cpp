// (C) 2013-2015, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#include "manager.hpp"
#include "messages.hpp"
#include <string>
#include <algorithm>

float Manager::getManagerSalary(float _baseSalary)
{
	if(_baseSalary == 0.0)
		return getSalaryBonus();

	if (m_subordinateEmployee.size() > 0)
	{
		float sum = 0;

		for (Employee * _empl : m_subordinateEmployee)
		{
			sum += _empl->calculateSalaryPerMounth(_baseSalary);
			sum -= _empl->getSalaryBonus();
		}

		return 2 * (sum / m_subordinateEmployee.size()) + getSalaryBonus();
	}
	else
		return 0.0;

}

void Manager::setNewEmployee(Employee & _empl)
{
	std::string _fullName = _empl.getEmployeeName();
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
		throw std::logic_error(Messages::SubordinateAlreadyAdded);
	else
		m_subordinateEmployee.push_back(&_empl);
		
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

		m_subordinateEmployee.erase(it);
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

