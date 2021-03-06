/*
**
* BEGIN_COPYRIGHT
*
* This file is part of SciDB.
* Copyright (C) 2008-2014 SciDB, Inc.
*
* SciDB is free software: you can redistribute it and/or modify
* it under the terms of the AFFERO GNU General Public License as published by
* the Free Software Foundation.
*
* SciDB is distributed "AS-IS" AND WITHOUT ANY WARRANTY OF ANY KIND,
* INCLUDING ANY IMPLIED WARRANTY OF MERCHANTABILITY,
* NON-INFRINGEMENT, OR FITNESS FOR A PARTICULAR PURPOSE. See
* the AFFERO GNU General Public License for the complete license terms.
*
* You should have received a copy of the AFFERO GNU General Public License
* along with SciDB.  If not, see <http://www.gnu.org/licenses/agpl-3.0.html>
*
* END_COPYRIGHT
*/

/*
 * LogicalSum.cpp
 * Example query : sum (scan ('array_to_scan'), 'attribute_to_aggregate' )
 *  Created on: Mar 18, 2010
 *      Author: hkimura
 *      TODO : so far, this class always returns double type.
 *      TODO : so far, this class doesn't accept group-by.
 *      TODO : count/avg/sum/min/max could be refactored to share their codes. almost same, after all.
 */

#include "query/Operator.h"
#include "system/Exceptions.h"
#include "query/Aggregate.h"

using namespace std;

namespace scidb
{

/**
 * @brief The operator: sum().
 *
 * @par Synopsis:
 *   sum( srcArray [, attr {, groupbyDim}*] )
 *
 * @par Summary:
 *   Produces a result array containing the sum value among the values of an attribute.
 *   If a list of groupbyDims is provided, one sum value is calculated for every distinct group.
 *
 * @par Input:
 *   - srcArray: a source array with srcAttrs and srcDims.
 *   - 0 or 1 attributes from srcAttrs. If no attribute is provided, the first attribute is used.
 *   - an optional list of groupbyDims.
 *
 * @par Output array:
 *        <
 *   <br>   attr_sum: the source attribute name, followed by '_sum'.
 *   <br> >
 *   <br> [
 *   <br>   groupbyDims (if a list of groupbyDims is provided); or
 *   <br>   i: start=0, end=0, chunk interval=1.
 *   <br> ]
 *
 * @par Examples:
 *   n/a
 *
 * @par Errors:
 *   n/a
 *
 * @par Notes:
 *   - same as aggregate(srcArray, sum(attr), groupbyDims).
 *
 */
class LogicalSum : public  LogicalOperator
{
public:
    LogicalSum(const std::string& logicalName, const std::string& alias):
        LogicalOperator(logicalName, alias)
    {
        ADD_PARAM_INPUT()
        ADD_PARAM_VARIES()
    }

    std::vector<boost::shared_ptr<OperatorParamPlaceholder> > nextVaryParamPlaceholder(const std::vector< ArrayDesc> &schemas)
    {
        std::vector<boost::shared_ptr<OperatorParamPlaceholder> > res;
        res.push_back(END_OF_VARIES_PARAMS());
        if (_parameters.size() == 0)
            res.push_back(PARAM_IN_ATTRIBUTE_NAME("void"));
        else
            res.push_back(PARAM_IN_DIMENSION_NAME());
        return res;
    }

    ArrayDesc inferSchema(std::vector< ArrayDesc> schemas, boost::shared_ptr< Query> query)
    {
        assert(schemas.size() == 1);
        ArrayDesc const& desc = schemas[0];
        Dimensions const& dims = desc.getDimensions();
        Attributes const& attrs = desc.getAttributes();
        AttributeID aid = 0;
        if (_parameters.size() >= 1)
        {
            aid = ((boost::shared_ptr<OperatorParamReference>&)_parameters[0])->getObjectNo();
        }
        AggregatePtr sumAggregate = AggregateLibrary::getInstance()->createAggregate("sum", TypeLibrary::getType(attrs[aid].getType()));
        Attributes aggAttrs(1);
        aggAttrs[0] = AttributeDesc((AttributeID)0,
                                    attrs[aid].getName() + "_sum",
                                    sumAggregate->getResultType().typeId(),
                                    AttributeDesc::IS_NULLABLE,
                                    0);

        if (_parameters.size() <= 1) { 
            Dimensions aggDims(1);
            aggDims[0] = DimensionDesc("i", 0, 0, 0, 0, 1, 0);
            return ArrayDesc(desc.getName(), aggAttrs, aggDims);
        } else { 
            vector<int> groupBy(_parameters.size()-1);
            size_t i;
            for (i = 0; i < groupBy.size(); ++i)
            {
                groupBy[i] = ((boost::shared_ptr<OperatorParamReference>&)_parameters[i + 1])->getObjectNo();
            }
            Dimensions aggDims(groupBy.size());
            for (size_t i = 0, n = aggDims.size(); i < n; i++) { 
                DimensionDesc const& srcDim = dims[groupBy[i]]; 
                aggDims[i] = DimensionDesc(  srcDim.getBaseName(),
                                             srcDim.getNamesAndAliases(),
                                             srcDim.getStartMin(),
                                             srcDim.getCurrStart(),
                                             srcDim.getCurrEnd(),
                                             srcDim.getEndMax(),
                                             i == 0 && groupBy[i] == 0 ? srcDim.getChunkInterval() : srcDim.getCurrLength(),
                                             0);
            }
            return ArrayDesc(desc.getName(), aggAttrs, aggDims);
        }
    }
};

DECLARE_LOGICAL_OPERATOR_FACTORY(LogicalSum, "sum")

} //namespace scidb

