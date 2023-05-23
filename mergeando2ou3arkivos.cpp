#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <boost\program_options.hpp>

//Programa para mergear 2 arkivos - Claudia Cavalcante Fonseca
short int par_size;
short int pos;
char*ultimoreg=NULL;

namespace po = boost::program_options; 
using namespace po;

class arkivo
      {private:
       std::string endereco;
       public:
       bool fim;
       FILE*registroatual;    
       fpos_t posicao;
       arkivo(std::string end,std::string tipo="rw") /*tipo pode ser r ou w ou rw*/
              {endereco=end;fim=0;
               if (endereco!="")
                  {if ((registroatual=fopen(endereco.c_str(),tipo.c_str()))==NULL && feof==0)
                      {throw 0;}}
               else registroatual=stdout;     
               }         
       char* ler()
/* referências: http://geekrepublic.pastebin.com/f2f654407 */
              {/*declaraçao de variaveis*/
               char *registro=NULL;
               short int tamanho = 0;
               while(tamanho==0)
                  {/*posição atual do arkivo =) e tamanho da linha - a função le todos os chars ateh \n e retorna um %d*/
                   fgetpos(registroatual, &posicao);
                   /*contagem das letras do primeiro registro*/
               	   fscanf(registroatual, "%*[^\n]%n", &tamanho); /* por algum motivo, precisa ser %n o.Ox */
                   /*alocaçao de memoria*/
                   registro = (char*)malloc(tamanho + 2);
            	      /*se o pontero for nulo (tiver dado erro no maloc)*/
        	          if(!registro) return "2";    	     
                   fsetpos(registroatual, &posicao);
                   fgets(registro,tamanho+2,registroatual);
                   /*fim do arkivo,retorna 1 * vide tabela abaixo **/  
                   registro[tamanho+2]='\0';
                   if (fim) return "1";
                   if (feof(registroatual))
                      {fim=!fim; if (registro[tamanho]!='\n'&& tamanho!=0) {registro[tamanho]='\n'; registro[tamanho+1]=='\0';};}
                   for (int i=tamanho;i>=0;i--)
                       {registro[i+1]=registro[i];}
                   registro[0]='0';
                   }
               return (registro);
        /*cohdigos de erros:
        0-a funçao deu certo
        1-Fim de Arquivo          
        2-Erro*/               
                };   
        /* o destrutor libera o arkivo aberto =) */           
        ~arkivo() {if (registroatual!=stdout) {fclose(registroatual);};}
       };
char*chave(char*registro)
       {char*xave=(char*)malloc(par_size+1);
        short int i;
        for (i=0;i<par_size+1&&(registro[pos+i]!='\n');i++)
            {xave[i]=registro[pos+i];}
        for (;i<=par_size+1;i++)
            {xave[i]='\0';}    
        return xave;
        }                      
/*ordena 2 vetores de chars e devolve a posiçao do menor*/       
bool ordenar(char*a, char*b, bool m3=0)	 
       {int i=0;
        if (!m3){a=chave(a);b=chave(b);}
        do
          {i++;
           if (a[i]<b[i]) return 0;
           else if(a[i]>b[i])return 1;
           }
        while(a[i]!='\0');
        return 0;/*caso os 2 sejam iguais, retorna a*/
        };    
/*ordena 3vetores de char e devolve a posiçao do menor vetor (0,1 ou 2)*/
short int ordenar(char*a, char*b, char*c)
       {int i=0; /*começo o while de 1, pq o primero char tem q ser desconsiderado(eh o char de controle de final de arkivo)*/
        a=chave(a);b=chave(b);c=chave(c);
        do
          {i++;
           if (a[i]<b[i])
              {if (ordenar(a,c,1)) return 2; else return 0;}
           else if(a[i]>b[i])
                  {if (ordenar(b,c,1)) return 2; else return 1;}           
           }
        while(a[i]!='\0');
        if (ordenar(a,c,1)) return 2;
        else return 0;};  
void Mergear2(arkivo*Arkivo[3],bool unique) /* unique diz se eh unique ou nao - Arkivo eh um vetor de ponteiros para 3arkivos(os 2 d entrada e o de saihda)*/
       {char*ark[2];
        ark[0]=Arkivo[0]->ler();
        ark[1]=Arkivo[1]->ler();
        if (!unique)
           {while (ark[0][0]=='0' && ark[1][0]=='0')
                  {unique=ordenar((ark[0]),(ark[1])); /*aki a unique recebe o indice do arkivo q tem a menor string atual*/
                   fprintf(Arkivo[2]->registroatual,ark[unique]+1);
                   ark[unique]=Arkivo[unique]->ler();}
            if (ark[unique][0]!='0') {unique=!unique;}
            while(ark[unique][0]=='0')
                  {fprintf(Arkivo[2]->registroatual,(ark[unique]+1));
                   ark[unique]=Arkivo[unique]->ler();}
            }
        else 
           {while (ark[0][0]=='0' && ark[1][0]=='0')
                  {unique=ordenar((ark[0]),(ark[1]));
                   if (ultimoreg==NULL || strcmp(ultimoreg,chave(ark[unique])))
                      {fprintf(Arkivo[2]->registroatual,ark[unique]+1);
                       ultimoreg=chave(ark[unique]);}
                   ark[unique]=Arkivo[unique]->ler();}
            if (ark[unique][0]!='0') unique=!unique;
            while(ark[unique][0]=='0')
                  {if(ultimoreg==NULL ||strcmp(ultimoreg,chave(ark[unique])))
                     {fprintf(Arkivo[2]->registroatual,(ark[unique]+1));
                      ultimoreg=chave(ark[unique]);};
                   ark[unique]=Arkivo[unique]->ler();}
            }
        delete Arkivo[0];
        delete Arkivo[1];   
        };
void Mergear3(arkivo*Arkivo[4],bool unique)
       {char*ark[3];
        ark[0]=Arkivo[0]->ler();
        ark[1]=Arkivo[1]->ler();
        ark[2]=Arkivo[2]->ler();
        short int Menor;
        arkivo*restantes[3];
        if (!unique)
           {while (ark[0][0]=='0' && ark[1][0]=='0' && ark[2][0]=='0')
                  {Menor=ordenar((ark[0]),(ark[1]),(ark[2]));
                   fprintf(Arkivo[3]->registroatual,ark[Menor]+1);
                   ark[Menor]=Arkivo[Menor]->ler();}
            }       
        else {while (ark[0][0]=='0' && ark[1][0]=='0' && ark[2][0]=='0')
                   {Menor=ordenar((ark[0]),(ark[1]),(ark[2]));
                    if (ultimoreg==NULL || strcmp(ultimoreg,chave(ark[Menor])))
                       {fprintf(Arkivo[3]->registroatual,ark[Menor]+1);
                        ultimoreg=chave(ark[Menor]);}
                    ark[Menor]=Arkivo[Menor]->ler();}
              }
        delete Arkivo[Menor];
        switch (Menor) 
        {case 0: {restantes[0]=Arkivo[1]; restantes[1]=Arkivo[2];ark[0]=ark[1];ark[1]=ark[2];break;}
         case 1: {restantes[0]=Arkivo[0]; restantes[1]=Arkivo[2];ark[1]=ark[2];break;}
         case 2: {restantes[0]=Arkivo[0]; restantes[1]=Arkivo[1];break;}}
         for(Menor=0;ark[0][Menor]!='\0';Menor++); /*calcular o tamanho do ark[0] (sizeof não funcionou por ser um ponteiro [nem dereferenciando])*/
         fseek(restantes[0]->registroatual,-(Menor),SEEK_CUR);restantes[0]->fim=0;
         for(Menor=0;ark[1][Menor]!='\0';Menor++);
         fseek(restantes[1]->registroatual,-(Menor),SEEK_CUR);restantes[1]->fim=0;
         restantes[2]=Arkivo[3];
         Mergear2(restantes,unique);                                  
         };     
int main(int argc, char *argv[])
{options_description desc ( "Parametros Admitidos");
          std::string ark[4];
          bool unique=0;
          desc.options_description::add_options()
                  ("help,h", "Mensagens que auxiliam o manuseio do Software")
                  ("ffile,1", po::value<std::string>(&ark[0]), "(Obrigatorio)Informa o nome do primeiro arquivo do Merge.")
                  ("sfile,2", po::value<std::string>(&ark[1]), "(Obrigatorio)Informa o nome do segundo arquivo do Merge.")
                  ("tfile,3", po::value<std::string>(&ark[2]), "Informa o nome do terceiro arquivo do Merge.")
                  ("out,o", po::value<std::string>(&ark[3]), "Informa o nome do arquivo de saida.")
                  ("size,s", po::value<short int>(&par_size), "(Obrigatorio)Informa o tamanho da chave a ser comparada nos arquivos.")
                  ("position,p", po::value<short int>(&pos)->default_value(0), "Informa o lugar do inicio da chave a ser comparada nos arquivos. Se nao informada,sera seu inicio.")
                  ("unique,u", "Especifica sobre a unicidade da chave no arquivo de saida")
                  ;
           po::variables_map vm;
                  po::store(po::parse_command_line(argc, argv, desc), vm);
                  po::notify(vm);
                  if (vm.count("help"))
                        {std::cout << desc << "\n"; }
                  else {if (vm.count("unique"))
                           {unique=1;}      
                        if (!vm.count("ffile") || !vm.count("sfile") || !vm.count("size"))
                              {std::cout<<" Faltou a especificacao de algum parametro obrigatorio\n Digite \"help\" para a exibicao de uma lista.\n";}
                        else {arkivo*Arkivo[4];
                              try {Arkivo[0]=new arkivo(ark[0],"r");
                                   Arkivo[1]=new arkivo(ark[1],"r");
                                   if (!vm.count("tfile"))
                                      {Arkivo[2]=new arkivo(ark[3],"w");
                                       Mergear2(Arkivo,unique);                          
                                       delete Arkivo[2];    
                                       }                            
                                   else      
                                      {Arkivo[2]=new arkivo(ark[2],"r");
                                       Arkivo[3]=new arkivo(ark[3],"w");
                                       Mergear3(Arkivo,unique);                          
                                       delete Arkivo[3];
                                       }
                                  }
                        catch(int a){printf("Ocorreu um erro ao abrir o arquivo. \nInforme seu nome correto,por favor.\n");
                      }}    
                        }     
    return EXIT_SUCCESS;
}
//Como não podia deixar de ser; agradeço, honrada pela ajuda, a Johann Felipe Voigt -^__^-x
