/* 
 * Open Source Movement Analysis Library
 * Copyright (C) 2016, Moveck Solution Inc., all rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *     * Redistributions of source code must retain the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *     * Neither the name(s) of the copyright holders nor the names
 *       of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 *  
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __openma_maths_utils_h
#define __openma_maths_utils_h

#include "openma/base/timesequence.h"

bool _ma_maths_verify_timesequence(const ma::TimeSequence* ts, int type, int components, int offset);

namespace ma
{
namespace maths
{
  /**
   * Extract a Result Map<ArrayBase> object from the given TimeSequence @a ts.
   * The Result object will have @a componments columns. If necessary the position of the data to extract can be shifted.
   * It is also possible to specify the type of the TimeSequence by specifying a TimeSequence::Type value to @a type. If no type is required, you can let the value to -1.
   * In case the number of components or the shift is greater than the number of columns, a empty object will be returned, This is the same if the expected type is not the good one.
   * @tparam Result Type of the extraction.
   * @a tparam T Type of the TimeSequence (w/o const correctness). 
   * @relates Array
   * @ingroup openma_maths
   */
  template <typename Result, typename T>
  inline Result to_arraybase_derived(T* ts, int components, int offset = 0, int type = -1)
  {
    static_assert(std::is_base_of<ArrayBase<Result>, Result>::value, "The template parameter is not a derived class of ArrayBase.");
    static_assert(std::is_same<TimeSequence, typename std::remove_const<T>::type>::value, "The type of the first arguement is not TimeSequence.");
    if (_ma_maths_verify_timesequence(ts, type, components, offset))
      return Result(ts->samples(), ts->data() + ts->samples() * offset, ts->data() + ts->samples() * (ts->components()-1) );
    else
      return Result();
  };
  
  // ----------------------------------------------------------------------- //
  
  /**
   * Extract from a TimeSequence @a ts a Map<Array>> where the number of columns of the resulting array is used as the number of components to extract.
   * It is possible to specify a possible @a offset to shift the data to extract.
   * The use of @a type will verify if the TimeSequence has the requeted type.
   * @tparam N Number of columns to extract.
   * @relates Array
   * @ingroup openma_maths
   */
  template <int N>
  inline Map<Array<N>> to_array(TimeSequence* ts, int offset = 0, int type = -1)
  {
    return to_arraybase_derived<Map<Array<N>>>(ts, N, offset, type);
  };
  
  /**
   * Extract from a TimeSequence @a ts a Map<const Array>> where the number of columns of the resulting array is used as the number of components to extract.
   * It is possible to specify a possible @a offset to shift the data to extract.
   * The use of @a type will verify if the TimeSequence has the requeted type.
   * @tparam N Number of columns to extract.
   * @relates Array
   * @ingroup openma_maths
   */
   template <int N>
  inline Map<const Array<N>> to_array(const TimeSequence* ts, int offset = 0, int type = -1)
  {
    return to_arraybase_derived<Map<const Array<N>>>(ts, N, offset, type);
  };
  
  // ----------------------------------------------------------------------- //
  
  /**
   * Specialized extraction method where the resulting Map<Array> has 1 column.
   * It is possible to specify a possible @a offset to shift the data to extract.
   * The use of @a type will verify if the TimeSequence has the requeted type.
   * @relates Array
   * @ingroup openma_maths
   */
  inline Map<Scalar> to_scalar(TimeSequence* ts, int offset = 0, int type = -1)
  {
    return to_array<1>(ts,offset,type);
  };
  
  /**
   * Specialized extraction method where the resulting Map<const Array> has 1 column.
   * It is possible to specify a possible @a offset to shift the data to extract.
   * The use of @a type will verify if the TimeSequence has the requeted type.
   * @relates Array
   * @ingroup openma_maths
   */
  inline Map<const Scalar> to_scalar(const TimeSequence* ts, int offset = 0, int type = -1)
  {
    return to_array<1>(ts,offset,type);
  };
  
  // ----------------------------------------------------------------------- //
  
  /**
   * Specialized extraction method where the resulting Map<Array> has 3 columns.
   * It is possible to specify a possible @a offset to shift the data to extract.
   * The use of @a type will verify if the TimeSequence has the requeted type.
   * @relates Array
   * @ingroup openma_maths
   */
  inline Map<Vector> to_vector(TimeSequence* ts, int offset = 0, int type = -1)
  {
    return to_array<3>(ts,offset,type);
  };
  
  /**
   * Specialized extraction method where the resulting Map<const Array> has 3 columns.
   * It is possible to specify a possible @a offset to shift the data to extract.
   * The use of @a type will verify if the TimeSequence has the requeted type.
   * @relates Array
   * @ingroup openma_maths
   */
  inline Map<const Vector> to_vector(const TimeSequence* ts, int offset = 0, int type = -1)
  {
    return to_array<3>(ts,offset,type);
  };
  
  // ----------------------------------------------------------------------- //
  
  /**
   * Specialized extraction method where the resulting Map<Array> has 3 columns (as well as the input - no possible offset) and the type must be set to TimeSequence::Marker.
   * @relates Array
   * @ingroup openma_maths
   */
  inline Map<Position> to_position(TimeSequence* ts)
  {
    return to_vector(ts,0,TimeSequence::Marker);
  };
  
  /**
   * Specialized extraction method where the resulting Map<const Array> has 3 columns (as well as the input - no possible offset) and the type must be set to TimeSequence::Marker.
   * @relates Array
   * @ingroup openma_maths
   */
  inline Map<const Position> to_position(const TimeSequence* ts)
  {
    return to_vector(ts,0,TimeSequence::Marker);
  };
  
  // ----------------------------------------------------------------------- //
  
  /**
   * Specialized extraction method where the result is a Map<Pose> object (as well as the input - no possible offset) and the type must be set to TimeSequence::Pose.
   * @relates Pose
   * @ingroup openma_maths
   */
  inline Map<Pose> to_pose(TimeSequence* ts)
  {
    return to_arraybase_derived<Map<Pose>>(ts,12,0,TimeSequence::Pose);
  };
  
  /**
   * Specialized extraction method where the result is a Map<const Pose> object (as well as the input - no possible offset) and the type must be set to TimeSequence::Pose.
   * @relates Pose
   * @ingroup openma_maths
   */
  inline Map<const Pose> to_pose(const TimeSequence* ts)
  {
    return to_arraybase_derived<Map<const Pose>>(ts,12,0,TimeSequence::Pose);
  };
};
};

#endif // __openma_maths_utils_h