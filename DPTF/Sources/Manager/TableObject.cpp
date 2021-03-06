/******************************************************************************
** Copyright (c) 2013-2020 Intel Corporation All Rights Reserved
**
** Licensed under the Apache License, Version 2.0 (the "License"); you may not
** use this file except in compliance with the License.
**
** You may obtain a copy of the License at
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
** WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
**
** See the License for the specific language governing permissions and
** limitations under the License.
**
******************************************************************************/

#include "TableObject.h"
#include "StatusFormat.h"
#include "XmlNode.h"
#include "DataVaultPath.h"
using namespace std;

TableObject::TableObject(TableObjectType::Type type, vector<TableObjectField> fields)
	: m_type(type)
	, m_fields(fields)
{
	auto dataVaultString = DataVaultPathBasePaths::ExportRoot + "/" + TableObjectType::ToString(m_type);
	m_dataVaultPathMap.insert({"get", {{"override", dataVaultString}, {"dptf", dataVaultString}}});
	m_dataVaultPathMap.insert({"set", {{"override", dataVaultString}}});
}

TableObject::~TableObject()
{
}

const DptfBuffer& TableObject::getData() const
{
	return m_data;
}

void TableObject::setData(const DptfBuffer& data)
{
	m_data = data;
}

TableObjectType::Type TableObject::getType() const
{
	return m_type;
}

const map<string, vector<pair<string, string>>> TableObject::getDataVaultPathMap() const
{
	return m_dataVaultPathMap;
}

string TableObject::getXmlString()
{
	if (m_data.size())
	{
		union esif_data_variant* obj = (union esif_data_variant*)m_data.get();

		auto remain_bytes = m_data.size();
		auto resultRoot = XmlNode::createWrapperElement("result");

		auto revison = (u64)obj->integer.value;
		obj = (union esif_data_variant*)((u8*)obj + sizeof(*obj));

		remain_bytes -= sizeof(*obj);

		resultRoot->addChild(XmlNode::createDataElement("revision", StatusFormat::friendlyValue(revison)));

		while (remain_bytes >= sizeof(*obj))
		{
			auto rowRoot = XmlNode::createWrapperElement("row");
			for (auto field = m_fields.begin(); field != m_fields.end(); field++)
			{
				remain_bytes -= sizeof(*obj);
				if (field->m_fieldDataType == ESIF_DATA_UINT64)
				{
					UInt64 int64FieldValue = (u64)obj->integer.value;
					obj = (union esif_data_variant*)((u8*)obj + sizeof(*obj));
					rowRoot->addChild(
						XmlNode::createDataElement(field->m_fieldLabel, StatusFormat::friendlyValue(int64FieldValue)));
				}
				else if (field->m_fieldDataType == ESIF_DATA_STRING)
				{
					char* strFieldValue = (char*)((u8*)obj + sizeof(*obj));
					remain_bytes -= obj->string.length;
					obj = (union esif_data_variant*)((u8*)obj + (sizeof(*obj) + obj->string.length));
					rowRoot->addChild(XmlNode::createDataElement(field->m_fieldLabel, strFieldValue));
				}
			}
			resultRoot->addChild(rowRoot);
		}

		return resultRoot->toString();
	}
	else
	{
		return "TableObject is empty.";
	}
}
