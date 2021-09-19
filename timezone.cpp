#include <utility>
#include <ctime>
#include <cstdio> 
using namespace std;
pair<short, short> getLocalTimeZone() 
{
	time_t t = 0; //1970-1-1 00:00:00 in UTC
	struct tm* l = localtime(&t);
	short h = l->tm_hour, m = l->tm_min;
	if(l->tm_year + 1900 < 1970)
	{
		h -= 24;
		if(m != 0)
		{
			h++;
			m = 60 - m;
		}
	}
	return make_pair(h,m);
}

int main()
{
	auto zone = getLocalTimeZone();
	printf("UTC%+.2d:%.2d",zone.first,zone.second);
	return 0;
} 
