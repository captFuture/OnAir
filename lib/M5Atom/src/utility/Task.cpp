#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <string>

#include "Task.h"

static char tag[] = "Task";

m5Task::m5Task(std::string taskName, uint16_t taskSize, uint8_t priority)
{
	m_handle 	= nullptr;
	m_taskdata 	= nullptr;
	m_taskname	= taskName;
	m_tasksize 	= taskSize;
	m_priority 	= priority;
	m_coreid	= tskNO_AFFINITY;
}

m5Task::~m5Task()
{
}

void m5Task::runTask(void* pTaskInstance)
{
	m5Task* pTask = (m5Task*)pTaskInstance;
	ESP_LOGD(tag, ">> Task %s run", pTask->m_taskname.c_str());
	pTask->run(pTask->m_taskdata);
	ESP_LOGD(tag, "<< Task %s stop", pTask->m_taskname.c_str());
	pTask->stop();
}

void m5Task::start(void* taskData)
{
	if( m_handle != nullptr )
	{
		ESP_LOGD(tag, "[] Task %s is already running", m_taskname.c_str());
	}
	m_taskdata = taskData;
	::xTaskCreatePinnedToCore(&runTask, m_taskname.c_str(), m_tasksize, this, m_priority, &m_handle, m_coreid);
}

void m5Task::stop()
{
	if( m_handle == nullptr )
	{
		return;
	}
	xTaskHandle handleTemp = m_handle;
	m_handle = nullptr;
	::vTaskDelete(handleTemp);
}

void m5Task::delay(int ms)
{
	::vTaskDelay(ms/portTICK_PERIOD_MS);
}

void m5Task::setTaskSize( uint16_t size )
{
	m_tasksize = size;
}

void m5Task::setTaskPriority( uint8_t priority)
{
	m_priority = priority;
}

void m5Task::setTaskName(std::string name)
{
	m_taskname = name;
}

void m5Task::setCore(BaseType_t coreID)
{
	m_coreid = coreID;
}

