﻿using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.BinOpCalc
{
    internal class PowerCalculator : IFullBinaryOpDispatcher
    {
        #region IFullBinaryOpDispatcher Members

        #region DataObjects.ObjectNumber
        public DataObjects.IDataObject Calc(DataObjects.ObjectNumber arg_num1, DataObjects.ObjectNumber arg_num2)
        {
            double x = arg_num1.Value;
            double y = arg_num2.Value;
            if(x == 0.0 && y == 0.0)
            {
                return new DataObjects.ObjectError(ErrorValue.not_numeric);
            }
            else if(x == 0.0 && y < 0.0)
            {
                return new DataObjects.ObjectError(ErrorValue.division_by_zero);
            }
            else if(x < 0 && Math.Truncate(y) != y)
            {
                return new DataObjects.ObjectError(ErrorValue.not_numeric);
            }
            return new DataObjects.ObjectNumber(Math.Pow(x, y));
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectNumber arg_num, DataObjects.ObjectBoolean arg_bool)
        {
            return Calc(arg_num, new DataObjects.ObjectNumber(arg_bool.Value ? 1.0 : 0.0));
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectNumber arg_num, DataObjects.ObjectError arg_err)
        {
            return arg_err;
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectNumber arg_num, DataObjects.ObjectString arg_str)
        {
            return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectNumber arg_num, DataObjects.ObjectReference arg_range)
        {
            return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectNumber arg_num, DataObjects.ObjectEmpty arg_empty)
        {
            return Calc(arg_num, new DataObjects.ObjectNumber(0.0));
        }
        #endregion

        #region DataObjects.ObjectBoolean
        public DataObjects.IDataObject Calc(DataObjects.ObjectBoolean arg_bool, DataObjects.ObjectNumber arg_num)
        {
            return Calc(new DataObjects.ObjectNumber(arg_bool.Value ? 1.0 : 0.0), arg_num);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectBoolean arg_bool1, DataObjects.ObjectBoolean arg_bool2)
        {
            return Calc(new DataObjects.ObjectNumber(arg_bool1.Value ? 1.0 : 0.0), new DataObjects.ObjectNumber(arg_bool2.Value ? 1.0 : 0.0));
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectBoolean arg_bool, DataObjects.ObjectError arg_err)
        {
            return arg_err;
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectBoolean arg_bool, DataObjects.ObjectString arg_str)
        {
            return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectBoolean arg_bool, DataObjects.ObjectReference arg_range)
        {
            return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectBoolean arg_bool, DataObjects.ObjectEmpty arg_empty)
        {
            return Calc(new DataObjects.ObjectNumber(arg_bool.Value ? 1.0 : 0.0), new DataObjects.ObjectNumber(0.0));
        }
        #endregion

        #region DataObjects.ObjectError
        public DataObjects.IDataObject Calc(DataObjects.ObjectError arg_err, DataObjects.ObjectNumber arg_num)
        {
            return arg_err;
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectError arg_err, DataObjects.ObjectBoolean arg_bool)
        {
            return arg_err;
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectError arg_err1, DataObjects.ObjectError arg_err2)
        {
            return arg_err1;
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectError arg_err, DataObjects.ObjectString arg_str)
        {
            return arg_err;
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectError arg_err, DataObjects.ObjectReference arg_range)
        {
            return arg_err;
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectError arg_err, DataObjects.ObjectEmpty arg_empty)
        {
            return arg_err;
        }
        #endregion

        #region DataObjects.ObjectString
        public DataObjects.IDataObject Calc(DataObjects.ObjectString arg_str, DataObjects.ObjectNumber arg_num)
        {
            return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectString arg_str, DataObjects.ObjectBoolean arg_bool)
        {
            return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectString arg_str, DataObjects.ObjectError arg_err)
        {
            return new DataObjects.ObjectError(ErrorValue.wrong_value_type); ;
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectString arg_str1, DataObjects.ObjectString arg_str2)
        {
            return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectString arg_str, DataObjects.ObjectReference arg_range)
        {
            return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectString arg_str, DataObjects.ObjectEmpty arg_empty)
        {
            return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
        }
        #endregion

        #region ObjectReference
        public DataObjects.IDataObject Calc(DataObjects.ObjectReference arg_range, DataObjects.ObjectNumber arg_num)
        {
            return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectReference arg_range, DataObjects.ObjectBoolean arg_bool)
        {
            return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectReference arg_range, DataObjects.ObjectError arg_err)
        {
            return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectReference arg_range, DataObjects.ObjectString arg_str)
        {
            return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectReference arg_range1, DataObjects.ObjectReference arg_range2)
        {
            return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectReference arg_range1, DataObjects.ObjectEmpty arg_empty)
        {
            return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
        }
        #endregion

        #region DataObjects.ObjectEmpty
        public DataObjects.IDataObject Calc(DataObjects.ObjectEmpty arg_empty, DataObjects.ObjectNumber arg_num)
        {
            return Calc(new DataObjects.ObjectNumber(0.0), arg_num);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectEmpty arg_empty, DataObjects.ObjectBoolean arg_bool)
        {
            return Calc(new DataObjects.ObjectNumber(0.0), new DataObjects.ObjectNumber(arg_bool.Value ? 1.0 : 0.0));
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectEmpty arg_empty, DataObjects.ObjectError arg_err) // Call inversed
        {
            return Calc(arg_err, arg_empty);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectEmpty arg_empty, DataObjects.ObjectString arg_str) // Call inversed
        {
            return Calc(arg_str, arg_empty);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectEmpty arg_empty, DataObjects.ObjectReference arg_range) // Call inversed
        {
            return Calc(arg_range, arg_empty);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectEmpty arg_empty1, DataObjects.ObjectEmpty arg_empty2)
        {
            return new DataObjects.ObjectError(ErrorValue.not_numeric);
        }

        #endregion

        #endregion
    }
}
