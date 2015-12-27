// (C) 2013-2015, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#include "controller.hpp"
#include "messages.hpp"

#include <stdexcept>
#include <algorithm>
#include <string>
#include <unordered_set>


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

int Controller::isEmployeeOrManager(std::string const & _fullname)
{
	auto it_empl = getRegularEmployeeNumberAtContainer(_fullname);
	auto it_mng = getManagerNumberAtContainer(_fullname);

	if (it_empl != m_regularEmployee.end())
		return 1;
	else if (it_mng != m_managers.end())
		return 2; 
	else
		throw std::logic_error(Messages::UnregisteredEmployeeName);
}

void Controller::assignEmployeeToManager(std::string const & _employeeName, std::string const & _managerName)
{
	auto it_empl = getRegularEmployeeNumberAtContainer(_employeeName);
	auto it_mng = getManagerNumberAtContainer(_managerName);

	if ((*it_empl)->getManagerName() == "")
	{
		if (it_mng != m_managers.end())
		{
			(*it_mng)->setNewEmployee(*(*it_empl));
			(*it_empl)->setManagerName(_managerName);
		}
	}
	else if ((*it_empl)->getManagerName() == _managerName)
		throw std::logic_error(Messages::SubordinateAlreadyAdded);
	else 
	{
		auto it_mngToDel = getManagerNumberAtContainer((*it_empl)->getManagerName());
		if (it_mngToDel != m_managers.end())
		{
			(*it_mngToDel)->deleteEmployee(_employeeName);
			(*it_mng)->setNewEmployee(*(*it_empl));
			(*it_empl)->setManagerName(_managerName);
		}
	}
}

void Controller::assignManagerToManager(std::string const & _employeeName, std::string const & _managerName)
{
	auto it_mngEmployee = getManagerNumberAtContainer(_employeeName);
	auto it_mng = getManagerNumberAtContainer(_managerName);

	if ((*it_mngEmployee)->getManagerName() == "")
	{
		(*it_mng)->setNewEmployee(*(*it_mngEmployee));
		(*it_mngEmployee)->setManagerName(_managerName);
	}
	else
	{
		auto it_mngToDel = getManagerNumberAtContainer((*it_mngEmployee)->getManagerName());
		if (it_mngToDel != m_managers.end())
		{
			(*it_mngToDel)->deleteEmployee(_employeeName);
			(*it_mng)->setNewEmployee(*(*it_mngEmployee));
			(*it_mngEmployee)->setManagerName(_managerName);
		}
	}
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
	auto it_empl = getRegularEmployeeNumberAtContainer(_fullName);
	auto it_mng = getManagerNumberAtContainer(_fullName);

	
	if (it_empl != m_regularEmployee.end())
		return true; 
	else if (it_mng != m_managers.end())
		return true;

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
		return (*it_mng)->getManagerSalary(_baseSalary);
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
	auto it_mng = getManagerNumberAtContainer(_fullName);
	if (it_empl != m_regularEmployee.end())
		return (*it_empl)->setSalaryBonus(_newBonus);
	else if (it_mng != m_managers.end())
		return (*it_mng)->setSalaryBonus(_newBonus);
	else
		throw std::logic_error(Messages::UnregisteredEmployeeName);
}


double Controller::getEmployeeBonus(std::string const & _fullName) const
{
	auto it_empl = getRegularEmployeeNumberAtContainer(_fullName);
	auto it_mng = getManagerNumberAtContainer(_fullName);
	if (it_empl != m_regularEmployee.end())
		return (*it_empl)->getSalaryBonus();
	else if (it_mng != m_managers.end())
		return (*it_mng)->getSalaryBonus();
	else
		throw std::logic_error(Messages::UnregisteredEmployeeName);
}

// first find in managers, then find at employee
std::string Controller::getEmployeeManager(std::string const & _fullName) const
{
	auto it_empl = getRegularEmployeeNumberAtContainer(_fullName);
	auto it_mng = getManagerNumberAtContainer(_fullName);

	if (it_empl != m_regularEmployee.end())
		return (*it_empl)->getManagerName();
	else if (it_mng != m_managers.end())
		return (*it_mng)->getManagerName();
	else
		throw std::logic_error(Messages::UnregisteredEmployeeName);
}


std::unordered_set<std::string> Controller::getManagerSubordinates(std::string const & _fullName) const
{
	auto it_empl = getRegularEmployeeNumberAtContainer(_fullName);
	if (it_empl != m_regularEmployee.end())
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
		throw std::logic_error(Messages::UnregisteredEmployeeName);
		//return _employeeNames;
}


void Controller::assignManager(std::string const & _employeeFullName, std::string const & _managerFullName)
{
	int a = isEmployeeOrManager(_employeeFullName);

	auto it_mng = getManagerNumberAtContainer(_managerFullName);
	if (it_mng == m_managers.end())
	{
		auto it_empl = getRegularEmployeeNumberAtContainer(_managerFullName);
		if (it_empl != m_regularEmployee.end())
			throw std::logic_error(Messages::NotAManager);
		else
			throw std::logic_error(Messages::UnregisteredEmployeeName);
	}

	if (a == 1)
	{
		assignEmployeeToManager(_employeeFullName, _managerFullName);
	}
	else if (a == 2)
	{
		assignManagerToManager(_employeeFullName, _managerFullName);
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

	if (it_empl != m_regularEmployee.end())
		(*it_empl)->levelUp();
	else if (it_mng != m_managers.end())
		throw std::logic_error(Messages::CannotPromoteManagers);
	else
		throw std::logic_error(Messages::UnregisteredEmployeeName);
}


void Controller::fire(std::string const & _employeeFullName)
{
	auto it_mng = getManagerNumberAtContainer(_employeeFullName);
	auto it_empl = getRegularEmployeeNumberAtContainer(_employeeFullName);

	if (it_empl != m_regularEmployee.end())
	{
		std::string _managerName = (*it_empl)->getManagerName();
		if (_managerName == "")
			m_regularEmployee.erase(it_empl);
		else
		{
			auto it_mngDel = getManagerNumberAtContainer(_managerName);
			if (it_mngDel != m_managers.end())
			{
				(*it_mngDel)->deleteEmployee(_employeeFullName);
				m_regularEmployee.erase(it_empl);
			}
		}
	}
	else if (it_mng != m_managers.end())
	{
		if ((*it_mng)->getSubordinateEmployee().size() > 0)
			throw std::logic_error(Messages::CannotFireAssignedManager);
		else
			m_managers.erase(it_mng);
	}
	else
		throw std::logic_error(Messages::UnregisteredEmployeeName);
}


