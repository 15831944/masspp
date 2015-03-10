/// <summary>
/// [FILE] SimcaDisplayPlugin.cs
/// [ABSTRACT] SIMCA Plugin Mass++ access class
/// Copyright (C) 2012-12-18 Shimadzu
/// </summary>
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

// Mass++ namespace
using kome.clr;

namespace SimcaDisplay
{
    /// <summary>
    /// SimcaDisplayPlugin class
    /// </summary>
    internal class SimcaDisplayPlugin
    {
        /**
         * @fn kome.clr.ClrVariant SimcaDisplayPluginFunction(kome.clr.ClrParameters* clrParams)
         * @brief write this function's brief.
         * @param[in] clrParams ClrParameters object
         * @return If true, it succeeded (boolean)
         */

        /// <summary>
        /// Mass++ menu(SimcaDisplay...) execute function
        /// </summary>
        /// <param name="clrParams">parameters object</param>
        /// <returns>This value has no meanings.</returns>
        public ClrVariant SimcaDisplayPluginFunction(ClrParameters clrParams)
        {
            // Return value (Success: true, Failure: false)
            ClrVariant ret = new ClrVariant();
            // We initially set ret.prim.boolVal to false so that we can return a false
            // result easily using "return ret" if a failure condition occurs.
            ret.type = ClrVariant.DataType.BOOL;
            ret.obj = false;

            // Method write Here
            SimcaDisplayManager.DisplayMainForm();
            
            ret.obj = true;
            return ret;
        }
    }
}
