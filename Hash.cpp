#include <bits/stdc++.h>
using namespace std;
map<unsigned int,string>  map_for_hashing;
int start_s;
int stop_s;
int cnt_lines;
unsigned long create_hash_value(string temp_str)
{
	unsigned long hash_val = 5381;
	for(int i=0;i<temp_str.size();i++)
	{
		if(temp_str[i]!=',')
			hash_val=((hash_val << 5) + hash_val) + temp_str[i];
	}
	return hash_val;
}

void file_close(ifstream& ipfilename)
{
//	cout<<"entered close"<<endl;
	ipfilename.close();
//	opfilename.close();
//	stop_s=clock();
}

void Getnext_hash(ifstream& inputfile,int no_of_buffer,int no_of_attr,int buffer_size)
{
	//int cnt_lines=0;
	start_s=clock();
	vector<string> ipbuffer;
	vector<unsigned int> opbuffer;
	map<unsigned int,string>::iterator m;
	long int records_ip_buffer=((no_of_buffer-1)*buffer_size)/(no_of_attr*4);
	long int records_op_buffer=buffer_size/(no_of_attr*4);
	long int cnt_ip_buffer=0;
	long int cnt_op_buffer=0;
	string lines;
	unsigned int hash_val;
	vector<string>::iterator ip_it;
	vector<unsigned int>::iterator op_it;
	while(getline(inputfile,lines))
	{
		if(cnt_ip_buffer<records_ip_buffer-1)
		{
			ipbuffer.push_back(lines);
			cnt_ip_buffer++;
		}
		else
		{
			ipbuffer.push_back(lines);
			cnt_ip_buffer++;
			for(ip_it=ipbuffer.begin();ip_it!=ipbuffer.end();++ip_it)
			{
				hash_val=create_hash_value(*ip_it);
//				cout<<hash_val<<endl;
				m=map_for_hashing.find(hash_val);
				if(m==map_for_hashing.end())
				{
//					cout<<"entered"<<endl;
					opbuffer.push_back(hash_val);
//					cout<<*op_it<<endl;
					cnt_op_buffer++;
//					cout<<"before hashing"<<endl;
					map_for_hashing.insert(pair<unsigned int,string>(hash_val,*ip_it));
//					cout<<"after hashing"<<endl;
				}
				if(cnt_op_buffer>records_op_buffer)
				{
//					cout<<"entered 3rd case"<<endl;
					for(op_it=opbuffer.begin();op_it!=opbuffer.end();++op_it)
					{	
//						outputfile<<map_for_hashing[*op_it]<<endl;
						cnt_lines++;
					}
					opbuffer.clear();
					cnt_op_buffer=0;
				}
			}
			cnt_ip_buffer=0;
			ipbuffer.clear();
		}
	}
//	cout<<cnt_ip_buffer<<endl;
	if(cnt_op_buffer>0)
	{
		for(op_it=opbuffer.begin();op_it!=opbuffer.end();op_it++)
		{	
//			outputfile<<map_for_hashing[*op_it]<<endl;
			cnt_lines++;
		}
	}
//	cout<<map_for_hashing.size()<<endl;
//	for(m=map_for_hashing.begin();m!=map_for_hashing.end();m++)
//		cout<<m->first<<":"<<m->second<<endl;
//	for(op_it = opbuffer.begin(); op_it != opbuffer.end(); op_it++) {
// 		cout << *op_it <<endl;
//  	}
//	cout<<count<<endl;
	stop_s=clock();
	file_close(inputfile);
	return ;
}

void file_open(string ipfilename,int no_of_attr,int no_of_buffer,int buffer_size)
{
//	cout<<"entered file func"<<endl;
	ifstream inputfile;
	inputfile.open(ipfilename.c_str());
//	ofstream outputfile ;
//	outputfile.open(opfilename.c_str());	
//	start_s=clock();
	Getnext_hash(inputfile,no_of_buffer,no_of_attr,buffer_size);
	return;
}

int main(int argc,char * argv[])
{
	if(argc<4)
	{
		exit(0);
	}
	else
	{
		cnt_lines=0;
		string inputfile=argv[2];
		int number_of_attr=10;
		int number_of_buffer=atoi(argv[1]);
		char *token=strtok(argv[3],"M");
		int buffer_size=atof(token);
		//string opfilename="../output/output.txt";
		string arguments;
		file_open(inputfile,number_of_attr,number_of_buffer,buffer_size);
		//ofstream outputfile ;
		//outputfile.open(opfilename.c_str(),ios::app);
		cout << "time:"<<(stop_s-start_s)/double(CLOCKS_PER_SEC)<< " " << cnt_lines << endl;
		//outputfile.close();
	}
	return 0;
}