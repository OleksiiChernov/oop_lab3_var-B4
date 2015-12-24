// (C) 2013-2015, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#include "controller.hpp"
#include "messages.hpp"

#include <stdexcept>
#include <algorithm>
#include <string>


Controller::~Controller()
{
	for (Manager * _mng : m_managers)
		delete _mng;

	for (RegularEmployee * _emp : m_regularEmployee)
		delete _emp;
}

auto Controller::getRegularEmployeeNumberAtContainer(std::string const & _fullName) const
{
	auto itr = std::find_if(m_regularEmployee.begin(), m_regularEmployee.end(), 
		[_fullName](RegularEmployee const * _empl)
		{
			if (_empl->getEmployeeName() == _fullName)
				return true;
			else
				return false;
		}
	);

	if (itr != m_regularEmployee.end())
	{
		return itr;
	}
	else
		return itr = m_regularEmployee.end();
}

auto Controller::getManagerNumberAtContainer(std::string const & _fullName) const
{
	auto itr = std::find_if(m_managers.begin(), m_managers.end(), 
		[_fullName](Manager const * _mng)
	{
		if (_mng->getEmployeeName() == _fullName)
			return true;
		else
			return false;
	}
	);
	if (itr != m_managers.end())
	{
		return itr;
	}
	else
		return itr = m_managers.end();
}



void Controller::createManager(std::string const & _fullName)
{
	auto it_empl = getRegularEmployeeNumberAtContainer(_fullName);

	if (it_empl != m_regularEmployee.end())
		throw std::logic_error(Messages::EmployeeFullNameNonUnique);
	else
	{

		auto it_mng = getManagerNumberAtContainer(_fullName);

		if (it_mng == m_managers.end())
		{
			Manager * mng = new Manager(_fullName);
			m_managers.push_back(mng);
		}
		else
			throw std::logic_error(Messages::EmployeeFullNameNonUnique);
	}
}


void Controller::createRegular(std::string const & _fullName, RegularEmployeeLevel _startingLevel)
{
	auto it_mng = getManagerNumberAtContainer(_fullName);

	if (it_mng != m_managers.end())
		throw std::logic_error(Messages::EmployeeFullNameNonUnique);
	else
	{
		auto it_empl = getRegularEmployeeNumberAtContainer(_fullName);

		if (it_empl == m_regularEmployee.end())
		{
			RegularEmployee * empl = new RegularEmployee(_fullName, _startingLevel);
			m_regularEmployee.push_back(empl);
		}
		else
			throw std::logic_error(Messages::EmployeeFullNameNonUnique);
	}
}


bool Controller::isKnownEmployee(std::string const & _fullName) const
{
	auto it_mng = getManagerNumberAtContainer(_fullName);

	if (it_mng != m_managers.end())
		return true;
	else
	{
		auto it_empl = getRegularEmployeeNumberAtContainer(_fullName);
		if (it_empl != m_regularEmployee.end())
			return true;
	}

	return false;
}


RegularEmployeeLevel Controller::getRegularEmployeeLevel(std::string const & _fullName) const
{
	auto it_mng = getManagerNumberAtContainer(_fullName);
	if (it_mng != m_managers.end())
		throw std::logic_error(Messages::NotARegular);
	else
	{
		auto it_empl = getRegularEmployeeNumberAtContainer(_fullName);
		if (it_empl != m_regularEmployee.end())
			return (*it_empl)->getEmployeeLevel();
		else
			throw std::logic_error(Messages::UnregisteredEmployeeName);
	}
}


double Controller::getEmployeeSalary(std::string const & _fullName, double _baseSalary)
{
	auto it_empl = getRegularEmployeeNumberAtContainer(_fullName);
	auto it_mng = getManagerNumberAtContainer(_fullName);
	if (it_empl != m_regularEmployee.end())
		return (*it_empl)->calculateSalaryPerMounth(_baseSalary);
	else if (it_mng != m_managers.end())
		return (*it_mng)->calculateSalaryPerMounth(_baseSalary);
	else
		throw std::logic_error(Messages::UnregisteredEmployeeName);
}


double Controller::getTotalSalary(double _baseSalary) const
{
	double _salary = 0;

	for (RegularEmployee * _emp : m_regularEmployee)
		_salary += _emp->calculateSalaryPerMounth(_baseSalary);

	//for (Manager * _mng : m_managers)
	//	_salary += _mng->calculateSalaryPerMounth(_baseSalary);

	return _salary;
}

void Controller::changeBonus(std::string const & _fullName, double _newBonus)
{
	auto it_empl = getRegularEmployeeNumberAtContainer(_fullName);
	if(it_empl != m_regularEmployee.end())
		(*it_empl)->setSalaryBonus(_newBonus);
	else
		throw std::logic_error(Messages::UnregisteredEmployeeName);
}


double Controller::getEmployeeBonus(std::string const & _fullName) const
{
	auto it_empl = getRegularEmployeeNumberAtContainer(_fullName);
	if (it_empl != m_regularEmployee.end())
		return (*it_empl)->getSalaryBonus();
	else
		throw std::logic_error(Messages::UnregisteredEmployeeName);
}


std::string Controller::getEmployeeManager(std::string const & _fullName) const
{
	for (Manager * _mng : m_managers)
	{
		auto it = std::find_if(_mng->getSubordinateEmployee().begin(), _mng->getSubordinateEmployee().end(), 
			[_fullName](Employee const * _empl)
			{
				if (_empl->getEmployeeName() == _fullName)
					return true;
				else
					return false;
			}
		);
		if (it != _mng->getSubordinateEmployee().end())
			return _mng->getEmployeeName();
	}
	throw std::logic_error(Messages::UnregisteredEmployeeName);
}


std::unordered_set<std::string> Controller::getManagerSubordinates(std::string const & _fullName) const
{
	auto it_empl = getRegularEmployeeNumberAtContainer(_fullName);
	if (it_empl > m_regularEmployee.begin())
		throw std::logic_error(Messages::NotAManager);

	std::unordered_set <std::string> _employeeNames;
	
	auto it_mng = getManagerNumberAtContainer(_fullName);
	if (it_mng != m_managers.end())
	{
		if ((*it_mng)->getSubordinateEmployee().size() > 0)
		{
			for (Employee * _empl : (*it_mng)->getSubordinateEmployee())
				_employeeNames.insert(_empl->getEmployeeName());
			return _employeeNames;
		}
	}
	else
		return _employeeNames;
}


void Controller::assignManager(std::string const & _employeeFullName, std::string const & _managerFullName)
{
	auto it_empl = getRegularEmployeeNumberAtContainer(_managerFullName);
	if (it_empl >= m_regularEmployee.begin())
		throw std::logic_error(Messages::NotAManager);
	else
	{
		auto it_mng = getManagerNumberAtContainer(_managerFullName);
		if (it_mng != m_managers.end())
		{
			(*it_mng)->setNewEmployee(_employeeFullName);

			auto it_emplToSet = getRegularEmployeeNumberAtContainer(_employeeFullName);
			if (it_emplToSet >= m_regularEmployee.begin())
				(*it_emplToSet)->setManagerName(_managerFullName);
			else
			{
				auto it_mngSetMng = getManagerNumberAtContainer(_employeeFullName);
				if (it_mngSetMng >= m_managers.begin())
					(*it_mngSetMng)->setManagerName(_managerFullName);
			}
		}
		else
			throw std::logic_error(Messages::UnregisteredEmployeeName);
	}
}


void Controller::deassignManager(std::string const & _employeeFullName)
{
	auto it_empl = getRegularEmployeeNumberAtContainer(_employeeFullName);
	auto it_mng = getManagerNumberAtContainer(_employeeFullName);

	if (it_empl != m_regularEmployee.end())
	{
		if ((*it_empl)->getManagerName() != "")
		{
			for (Manager * _mng : m_managers)
			{
				if (_mng->hasSubordinatedEmployee(_employeeFullName))
				{
					_mng->deleteEmployee(_employeeFullName);
					if (it_empl != m_regularEmployee.end())
					{
						(*it_empl)->delManager();
					}
					else
					{
						if (it_mng != m_managers.end())
						{
							(*it_mng)->delManager();
						}
					}
				}
			} // for Manager
		}
		else
			throw std::logic_error(Messages::EmployeeHasNoManager);
	}
	else if (it_mng != m_managers.end())
	{
		if ( (*it_mng)->getManagerName() != "")
		{
			for (Manager * _mng : m_managers)
			{
				if (_mng->hasSubordinatedEmployee(_employeeFullName))
				{
					_mng->deleteEmployee(_employeeFullName);
					if (it_empl != m_regularEmployee.end())
					{
						(*it_empl)->delManager();
					}
					else
					{
						if (it_mng != m_managers.end())
						{
							(*it_mng)->delManager();
						}
					}
				}
			} // for Manager
		}
		else
			throw std::logic_error(Messages::EmployeeHasNoManager);
	}
	else
		throw std::logic_error(Messages::UnregisteredEmployeeName);
		
} 


void Controller::promote(std::string const & _employeeFullName)
{
	auto it_empl = getRegularEmployeeNumberAtContainer(_employeeFullName);
	auto it_mng = getManagerNumberAtContainer(_employeeFullName);
	if (it_empl >= m_regularEmployee.begin() && it_empl != m_regularEmployee.end())
	{
		(*it_empl)->levelUp();
	}
	else if (it_mng >= m_managers.begin())
		throw std::logic_error(Messages::CannotPromoteManagers);
	else
		throw std::logic_error(Messages::UnregisteredEmployeeName);
}


void Controller::fire(std::string const & _employeeFullName)
{
	auto it_mng = getManagerNumberAtContainer(_employeeFullName);
	if (it_mng >= m_managers.begin() && it_mng != m_managers.end())
	{
		(*it_mng)->fireManager();

		if ((*it_mng)->getSubordinateEmployee().size() > 0)
			throw std::logic_error(Messages::CannotFireAssignedManager);
	}
	else
		throw std::logic_error(Messages::UnregisteredEmployeeName);
}


