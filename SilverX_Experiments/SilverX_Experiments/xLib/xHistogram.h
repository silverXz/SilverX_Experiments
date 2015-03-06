#pragma once

#include <limits> // for std::numeric_limits


// conflict with windows definition; wujh;

#ifdef   max   
#undef   max   
#endif 
#ifdef   min   
#undef   min   
#endif 

/**
* Histogram.
*
* This class implements a single-value histogram.
*/
template <class ScalarType> 
class Histogram 
{
	// public data members
private:

	std::vector <int> H; 	    //! Counters for bins.
	std::vector <ScalarType> R; //! Range for bins.
	ScalarType minv; 	        //! Minimum value.
	ScalarType maxv;	        //! Maximum value.
	int n;	//! Number of vaild intervals stored between minv and maxv.


	/// incrementally updated values
	int cnt;	    //! Number of accumulated samples.
	ScalarType avg;	//! Average.
	ScalarType rms; //! Root mean square.

	/** 
	* Returns the index of the bin which contains a given value.
	*/
	int BinIndex(ScalarType val) ;

	// public methods
public:

	/** 
	* Set the histogram values.
	*
	* This method is used to correctly initialize the bins of the histogram.
	* n is the number of valid intervals between minv and maxv. 
	* for a more robust working, the Histogram class stores also the two out of range 
	* intervals (-inf, minv] and [maxv, +inf)
	* Each bin is left closed (eg it contains the value 
	* The \a gamma parameter is applied to modify the distribution of the ranges of 
	* the bins. Default uniform distibution. 
	* 
	*/
	void SetRange(ScalarType _minv, ScalarType _maxv, int _n,ScalarType gamma=1.0 );

	ScalarType MinV() {return minv;}; 	//! Minimum value.
	ScalarType MaxV() {return maxv;}; 	//! Minimum value.

	/** 
	* Add a new value to the histogram.
	*
	* The statistics related to the histogram data (average, RMS, etc.) are 
	* also updated.
	*/
	void Add(ScalarType v);

	int MaxCount() const;
	int BinCount(ScalarType v);
	int BinCount(ScalarType v, ScalarType width);
	int RangeCount(ScalarType rangeMin, ScalarType rangeMax);
	ScalarType BinWidth(ScalarType v); 

	/** 
	* Returns the value corresponding to a given percentile of the data.
	*
	* The percentile range between 0 and 1.
	*/
	ScalarType Percentile(ScalarType frac) const;

	//! Returns the average of the data.
	ScalarType Avg(){ return avg/cnt;}

	//! Returns the Root Mean Square of the data.
	ScalarType RMS(){ return sqrt(rms/double(cnt));}

	//! Returns the variance of the data. // 方差:标准偏差数的平方
	ScalarType Variance(){ return fabs(rms/cnt-Avg()*Avg());}

	//! Returns the standard deviation of the data.
	ScalarType StandardDeviation(){ return sqrt(Variance());}

	//! Dump the histogram to a file.
	void FileWrite(const std::string &filename);

	//! Reset histogram data.
	void Clear();
};

template <class ScalarType> 
void Histogram<ScalarType>::Clear()
{
	H.clear();
	R.clear();
	cnt = 0;
	avg = 0;
	rms = 0;
	n = 0;
	minv = 0;
	maxv = 1;
}

/*
Note that the histogram holds <n> valid bins plus two semi-infinite bins.

R[0]   = -inf 
R[1]   = minv
R[n+1] = maxv
R[n+2] = +inf


Eg. SetRange(0, 10, 5) asks for 5 intervals covering the 0..10 range 

H[0]  H[1]   H[2]   H[3]   H[4]   H[5]    H[6]  
-inf    0      2      4      6      8      10    +inf 
R[0]   R[1]   R[2]   R[3]   R[4]   R[5]   R[6]   R[7]

*/

template <class ScalarType> 
void Histogram<ScalarType>::SetRange(ScalarType _minv, ScalarType _maxv, int _n, ScalarType gamma)
{
	// reset data
	Clear();

	minv = _minv; maxv = _maxv; n = _n;
	H.resize(n+2);
	fill(H.begin(), H.end(),0);
	R.resize(n+3);

	R[0]   = - std::numeric_limits< ScalarType >::max(); 
	R[n+2] =   std::numeric_limits< ScalarType >::max(); 

	ScalarType delta = (maxv - minv);
	if ( gamma == 1 )
	{
		for(int i=0; i<=n; ++i)
			R[i+1] = minv + delta*ScalarType(i)/n;
	}
	else
	{
		for(int i=0; i<=n; ++i)
			R[i+1] = minv + delta*pow(ScalarType(i)/n, gamma);
	}
}



template <class ScalarType> 
int Histogram<ScalarType>::BinIndex(ScalarType val) 
{
	// lower_bound returns the furthermost iterator i in [first, last) 
	// such that, for every iterator j in [first, i), *j < value.
	// E.g. An iterator pointing to the first element "not less than" val, 
	// or end() if every element is less than val.
	typename std::vector<ScalarType>::iterator it = lower_bound(R.begin(),R.end(),val);

	assert( it != R.begin() );
	assert( it != R.end() );
	assert( (*it) >= val );

	int pos = it-R.begin();
	assert(pos >=1);
	pos -= 1; 
	assert (R[pos] < val);
	assert (         val <= R[pos+1] );
	return pos;
}

/*
H[0]  H[1]   H[2]   H[3]   H[4]   H[5]    H[6]  
-inf    0      2      4      6      8      10    +inf 
R[0]   R[1]   R[2]   R[3]   R[4]   R[5]   R[6]   R[7]

asking for 3.14 lower bound will return an iterator pointing to R[3]==4; and will increase H[2] 
asking for 4    lower bound will return an iterator pointing to R[3]==4; and will increase H[2] 

*/
template <class ScalarType> 
void Histogram<ScalarType>::Add(ScalarType v)
{
	int pos = BinIndex(v);
	if( pos>=0 && pos<=n )
	{
		++H[pos];
		++cnt;
		avg += v;
		rms += v*v;
	}
}

template <class ScalarType> 
int Histogram<ScalarType>::BinCount(ScalarType v) 
{
	return H[BinIndex(v)];
}

template <class ScalarType> 
int Histogram<ScalarType>::BinCount(ScalarType v, ScalarType width) 
{
	return RangeCount(v-width/2.0, v+width/2.0);
}

template <class ScalarType> 
int Histogram<ScalarType>::RangeCount(ScalarType rangeMin, ScalarType rangeMax) 
{
	int firstBin = BinIndex(rangeMin);
	int lastBin = BinIndex (rangeMax);
	int sum=0;
	for(int i=firstBin; i<= lastBin; ++i)
		sum += H[i];
	return sum;
}

template <class ScalarType> 
ScalarType Histogram<ScalarType>::BinWidth(ScalarType v) 
{
	int pos = BinIndex(v);
	return R[pos+1]- R[pos];
}

template <class ScalarType> 
void Histogram<ScalarType>::FileWrite(const std::string &filename)
{
	FILE *fp;
	fp=fopen(filename.c_str(),"w");

	for(unsigned int i=0; i<H.size(); i++)
		fprintf (fp,"%12.8lf , %12.8lf \n",R[i+1],double(H[i])/cnt);

	fclose(fp);
}


template <class ScalarType> 
int Histogram<ScalarType>::MaxCount() const
{
	return *(std::max_element(H.begin(), H.end()));
}

// Return the scalar value <r> such that there are <frac> samples <= <r>. 
// E.g. Percentile(0.0) will return  R[1]   e.g. min value
// E.g. Percentile(1.0) will return  R[n+1] e.g max value

template <class ScalarType> 
ScalarType Histogram<ScalarType>::Percentile(ScalarType frac) const
{
	if(H.size()==0 && R.size()==0) 
		return 0;

	// check percentile range
	assert(frac >= 0 && frac <= 1);

	ScalarType sum=0, partsum=0;
	//int i;
	std::size_t i;

	// useless summation just to be sure 
	for( i=0; i<H.size(); i++ ) sum += H[i]; 
	assert( sum == cnt );

	sum*=frac;
	for(i=0; i<H.size(); i++)
	{
		partsum += H[i];
		if( partsum >= sum) break;
	}

	assert( i<H.size() );

	return R[i+1];
}

typedef Histogram<double> Histogramd ;
typedef Histogram<float> Histogramf ;

