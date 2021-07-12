#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;

class NFA
{
	public:
		int n, m;
		unsigned int s, f;
		int no_s, no_f;
		unsigned int *delta;
};

class DFA
{
	public:
		unsigned int n;
		int m;
		unsigned int s;
		unsigned int *f, *delta;
		int no_f;
};

NFA readNFA(string file_name)
{
    int i;
    unsigned int p, a, q;
	NFA N;
	ifstream in;
	in.open(file_name);
	int num;
	in>>num;
	N.n = num;
	in>>num;
	N.m = num;
	N.delta = new unsigned int[N.n*N.m];
	N.s = 0;	N.f = 0;
	N.no_s = 0;
	while(true)
	{
		in>>num;
		if(num == -1)
			break;
		N.s |= (1U<<num);
		N.no_s++;
	}
	N.no_f = 0;
	while(true)
	{
		in>>num;
		if(num == -1)
			break;
		N.f |= (1U<<num);
		N.no_f++;
	}
	for(i=0; i<N.n*N.m; i++)
		N.delta[i] = 0;
	while(true)
	{
		in>>num;
		if(num == -1)
			break;
		p = num;
		in>>num;	a = num;
		in>>num;	q = num;
		N.delta[p+a*N.n] |= (1U<<q);
	}
	return N;
}

DFA subsetcons(NFA N)
{
	DFA D;
	unsigned int i, temp, nn;
	int j, k, flag;
	D.n = pow(2,N.n);
	D.m = N.m;    D.s = N.s;
	D.delta = new unsigned int[D.n*D.m];
	for(i=0; i<D.n*D.m; i++)
		D.delta[i] = 0;
	for(i=0; i<D.n; i++)
	{
		for(j=0; j<D.m; j++)
		{
			temp = 0;
			for(k=0; k<32; k++)
				if(i & (1U<<k))
					temp = temp|N.delta[k+N.n*j];
			D.delta[i+D.n*j] = temp;
		}
	}
	if(D.n<=32)
	{
		D.f = new unsigned int[1];
		D.f[0] = 0;
		for(i=0; i<D.n; i++)
		{
			flag = 0;
			for(j=0; j<5; j++)
			{
				if((i & (1U<<j)) && (N.f & (1U<<j)))
				{
					flag = 1;
					break;
				}
			}
			if(flag == 1)
			{
				D.f[0] |= (1U<<i);
			}
		}
	}
	else
	{
		D.f = new unsigned int[D.n/32];
		nn = D.n/32;
		for(i=0; i<nn; i++)
			D.f[i] = 0;
		for(i=0; i<D.n; i++)
		{
			flag = 0;
			for(j=0; j<32; j++)
			{
				if((i & (1U<<j)) && (N.f & (1U<<j)))
				{
					flag = 1;
					break;
				}
			}
			if(flag == 1)
			{
				D.f[i/32] |= (1U<<(i%32));
			}
		}
	}
	D.no_f = D.n - D.n/(pow(2,N.no_f));
	return D;
}

void printNFA(NFA N)
{
	int i, j, k;
	unsigned int temp;
	cout<<"    Number of states: "<<N.n<<endl;
	cout<<"    Input alphabet: {0";
	for(i=1; i<N.m; i++)
		cout<<","<<i;
	cout<<"}"<<endl;
	cout<<"    Start states: {";
	for(i=0; i<32; i++)
	{
		if(N.s & (1U<<i))
		{
			cout<<i;
			break;
		}
	}
	for(i++; i<32; i++)
		if(N.s & (1U<<i))
			cout<<","<<i;
	cout<<"}"<<endl;
	cout<<"    Final states: {";
	for(i=0; i<32; i++)
	{
		if(N.f & (1U<<i))
		{
			cout<<i;
			break;
		}
	}
	for(i++; i<32; i++)
		if(N.f & (1U<<i))
			cout<<","<<i;
	cout<<"}"<<endl;
	cout<<"    Transition function"<<endl;
	for(i=0; i<N.n; i++)
	{
		for(j=0; j<N.m; j++)
		{
			cout<<"        Delta("<<i<<","<<j<<") = {";
			temp = N.delta[i+N.n*j];
			for(k=0; k<32; k++)
			{
				if(temp & (1U<<k))
				{
					cout<<k;
					break;
				}
			}
			for(k++; k<32; k++)
				if(temp & (1U<<k))
					cout<<","<<k;
			cout<<"}"<<endl;
		}
	}
	cout<<endl;
}

void printDFA(DFA D)
{
	unsigned int i;
	int j;
	cout<<"    Number of states: "<<D.n<<endl;
	cout<<"    Input alphabet: {0";
	for(j=1; j<D.m; j++)
		cout<<","<<j;
	cout<<"}"<<endl;
	cout<<"    Start state: "<<D.s<<endl;
	if(D.n <= 64)
	{
		cout<<"    Final states: {";
		if(D.n<=32)
		{
			for(i=0; i<D.n; i++)
			{
				if(D.f[0] & (1U<<i))
				{
					cout<<i;
					break;
				}
			}
			for(i++; i<D.n; i++)
				if(D.f[0] & (1U<<i))
					cout<<","<<i;
			cout<<"}"<<endl;
		}
		else
		{
			for(i=0; i<D.n; i++)
			{
				if(D.f[i/32] & (1U<<(i%32)))
				{
					cout<<i;
					break;
				}
			}
			for(i++; i<D.n; i++)
				if(D.f[i/32] & (1U<<(i%32)))
					cout<<","<<i;
			cout<<"}"<<endl;
		}
		cout<<"    Transition function:"<<endl;
		cout<<"    a/p|";
		for(i=0; i<D.n; i++)
		{
			if(i<10)
				cout<<" ";
			cout<<" "<<i;
		}
		cout<<endl;
		cout<<"    ---+";
		for(i=0; i<D.n; i++)
			cout<<"---";
		cout<<endl;
		for(j=0; j<D.m; j++)
		{
			cout<<"     "<<j<<" |";
			for(i=0; i<D.n; i++)
			{
				if(D.delta[i+D.n*j] < 10)
					cout<<" ";
				cout<<" "<<D.delta[i+D.n*j];
			}
			cout<<endl;
		}
		cout<<endl;
	}
	else
	{
		cout<<"    "<<D.no_f<<" final states"<<endl;
		cout<<"    Transition function: Skipped"<<endl;
		cout<<endl;
	}
}

void dfs(unsigned int p, DFA D, bool visited[], unsigned int R[], unsigned int r[])
{
	if(visited[p] == true)
		return;
	int j;
	visited[p] = true;
	R[r[0]] = p;
	r[0]++;
	for(j=0; j<D.m; j++)
		dfs(D.delta[p+D.n*j], D, visited, R, r);
}
void sort(unsigned int R[], int no_rs)
{
	int i, j;
	for(i=0; i<no_rs-1; i++)
		for(j=0; j<no_rs-i-1; j++)
			if(R[j] > R[j+1])
			{
				R[j]   = R[j] + R[j+1];
				R[j+1] = R[j] - R[j+1];
				R[j]   = R[j] - R[j+1];
			}
}
void findreachable(DFA D, unsigned int R[], unsigned int &no_rs)
{
	unsigned int i, *r;
	bool *visited;
	visited = new bool[D.n];
	for(i=0; i<D.n; i++)
		visited[i] = false;
	r = new unsigned int[1];
	r[0] = 0;
	dfs(D.s, D, visited, R, r);
	no_rs = r[0];
	sort(R,no_rs);
}

int getindex(unsigned int p, unsigned int R[], int no_rs)
{
	int j;
	for(j=0; j<no_rs; j++)
		if(p == R[j])
			return j;
	return -1;
}
DFA rmunreachable(DFA D, unsigned int R[], int no_rs)
{
	unsigned int i, len;
	int j;
	DFA reduced_D;
	reduced_D.n = no_rs;
	reduced_D.m = D.m;
	reduced_D.s = getindex(D.s, R, no_rs);
	reduced_D.delta = new unsigned int[reduced_D.n*reduced_D.m];
	for(j=0; j<reduced_D.m; j++)
		for(i=0; i<reduced_D.n; i++)
			reduced_D.delta[i+reduced_D.n*j] = getindex(D.delta[R[i]+D.n*j], R, no_rs);
	if(reduced_D.n%32 == 0)
		len = reduced_D.n/32;
	else
		len = reduced_D.n/32 + 1;
	reduced_D.f = new unsigned int[len];
	for(i=0; i<len; i++)
		reduced_D.f[i] = 0;
	reduced_D.no_f = 0;
	for(i=0; i<reduced_D.n; i++)
	{
		if(D.f[R[i]/32] & (1U<<(R[i]%32)))
		{
			reduced_D.f[i/32] |= (1U<<(i%32));
			reduced_D.no_f++;
		}
	}
	return reduced_D;
}

unsigned int belongs_to_F(unsigned int p, DFA D)
{
	return (D.f[p/32] & (1U<<(p%32)));
}
unsigned int not_belongs_to_F(unsigned int p, DFA D)
{
	return !belongs_to_F(p,D);
}
void findequiv(bool matrix[500][500], DFA D)
{
	int i, j, i1, j1, k, flag;
	for(i=1; i<D.n; i++)
		for(j=0; j<i; j++)
			matrix[i][j] = false;
	for(i=1; i<D.n; i++)
	{
		for(j=0; j<i; j++)
		{
			if(belongs_to_F(i,D) && not_belongs_to_F(j,D))
				matrix[i][j] = true;
			if(not_belongs_to_F(i,D) && belongs_to_F(j,D))
				matrix[i][j] = true;
		}
	}
	while(true)
	{
		flag = 0;
		for(i=1; i<D.n; i++)
		{
			for(j=0; j<i; j++)
			{
				if(matrix[i][j] == false)
				{
					for(k=0; k<D.m; k++)
					{
						i1 = D.delta[i+D.n*k];
						j1 = D.delta[j+D.n*k];
						if(i1>j1)
						{
							if(matrix[i1][j1] == true)
							{
								matrix[i][j] = true;
								flag = 1;
							}
						}
						if(i1<j1)
						{
							if(matrix[j1][i1] == true)
							{
								matrix[i][j] = true;
								flag = 1;
							}
						}
					}
				}
			}
		}
		if(flag==0)
			break;
	}
}

void print_eq_states(DFA D, unsigned int Group[500][500], int no_c)
{
	int j, l, k;
	k = no_c;
	cout<<"+++ Equivalent states"<<endl;
	for(j=0; j<k; j++)
	{
		cout<<"    Group ";
		if(j < 10)
			cout<<" ";
		cout<<j<<": {";
		for(l=0; l<D.n; l++)
		{
			if(Group[j][l/32] & (1U<<(l%32)))
			{
				cout<<l;
				break;
			}
		}
		for(l++; l<D.n; l++)
			if(Group[j][l/32] & (1U<<(l%32)))
				cout<<","<<l;
		cout<<"}"<<endl;
	}
	cout<<endl;
}
void equivalent_states(DFA D, bool matrix[500][500], unsigned int Group[500][500], int &no_c)
{
	unsigned int i, j, k, len;
	bool grouped[500];
	for(i=0; i<500; i++)
		for(j=0; j<500; j++)
			Group[i][j] = 0;
	for(i=0; i<500; i++)
		grouped[i] = false;
	k = 0;
	for(j=0; j<D.n; j++)
	{
		if(grouped[j] == false)
		{
			grouped[j] = true;
			Group[k][j/32] |= (1U<<(j%32));
			for(i=j+1; i<D.n; i++)
			{
				if(matrix[i][j] == false)
				{
					grouped[i] = true;
					Group[k][i/32] |= (1U<<(i%32));
				}
			}
			k++;
		}
	}
	no_c = k;
	print_eq_states(D,Group,no_c);
}

int getcollapsedindex(unsigned int p, unsigned int Group[500][500], int no_c)
{
	unsigned int i, k;
	k = no_c;
	for(i=0; i<k; i++)
		if(Group[i][p/32] & (1U<<(p%32)))
			return i;
	return -1;
}
DFA collapse(DFA D, unsigned int Group[500][500], int no_c)
{
	DFA CD;
	int i, j, k, l, len;
	k = no_c;
	CD.n = k;
	CD.m = D.m;
	CD.s = getcollapsedindex(D.s,Group,no_c);
	CD.delta = new unsigned int[CD.n*CD.m];
	for(i=0; i<CD.m*CD.n; i++)
		CD.delta[i] = 0;
	for(i=0; i<CD.n; i++)
	{
		for(j=0; j<CD.m; j++)
		{
			for(l=0; l<D.n; l++)
				if(Group[i][l/32] & (1U<<(l%32)))
					break;
			CD.delta[i+CD.n*j] = getcollapsedindex(D.delta[l+D.n*j], Group, k);
		}
	}
	if(CD.n%32 == 0)
		len = CD.n/32;
	else
		len = CD.n/32 + 1;
	CD.f = new unsigned int[len];
	for(i=0; i<len; i++)
		CD.f[i] = 0;
	CD.no_f = 0;
	for(i=0; i<CD.n; i++)
	{
		for(j=0; j<D.n; j++)
			if(Group[i][j/32] & (1U<<(j%32)))
				break;
		if(D.f[j/32] & (1U<<(j%32)))
		{
			CD.f[i/32] |= (1U<<(i%32));
			CD.no_f++;
		}
	}
	return CD;
}


int main()
{
	string file_name;
	cout<<"Enter the name of the file:\t";
	cin>>file_name;

	NFA N;
	N = readNFA(file_name);
	cout<<"+++ Input NFA"<<endl;
	printNFA(N);

	DFA D;
	D = subsetcons(N);
	cout<<"+++ Converted DFA"<<endl;
	printDFA(D);

	unsigned int i, *R, no_rs;
	R = new unsigned int[D.n];
	findreachable(D, R, no_rs);
	cout<<"+++ Reachable states: {"<<R[0];
	for(i=1; i<no_rs; i++)
		cout<<","<<R[i];
	cout<<"}"<<endl<<endl;

	DFA reduced_D;
	reduced_D = rmunreachable(D, R, no_rs);
	cout<<"+++ Reduced DFA after removing unreachable states"<<endl;
	printDFA(reduced_D);

	bool matrix[500][500];
	findequiv(matrix, reduced_D);

	int no_c;
	unsigned int Group[500][500];
	equivalent_states(reduced_D,matrix,Group,no_c);

	DFA collapsed_DFA;
	cout<<"+++ Reduced DFA after collapsing equivalent states"<<endl;
	collapsed_DFA = collapse(reduced_D,Group,no_c);
	printDFA(collapsed_DFA);


	return 1;
}