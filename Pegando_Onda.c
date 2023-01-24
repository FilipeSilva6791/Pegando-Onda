 /********************************************************************
  @brief  C�digo para manipula��o de arquivos de som no formato WAVE
  @author Filipe Pereira da Silva
  @date   08 de dezembro de 2020
  @version 1.0
  ********************************************************************/

  #include<stdio.h>
  #include<stdlib.h>
  #include<locale.h>
  #include<stdint.h>
  #include<string.h>

   int MetaDados (FILE *file);
   void PrintFourChar(char *ch);

  /************************************************************
   Cr�ditos : Trecho de c�digo inspirado no c�digo do professor
      Marcos Zuccoloto escrito na aula de 25/11/20           */

  //esta struct armazenar� os valores do cabe�alho
  //(o cabe�alho de um arquivo WAVE sempre � padr�o)
  //ver em : http://soundfile.sapp.org/doc/WaveFormat/

  typedef struct {

    char ChunkId[4];
    uint32_t ChunkSize;
    char format[4];
    char SubChunkId[4];
    uint32_t SubChunkSize;
    uint16_t AudioFormat;
    uint16_t NumChannels;
    uint32_t SampleRate;
    uint32_t ByteRate;
    uint16_t BlockAlign;
    uint16_t BitsPerSample;

  } dataField;
  /************************************************************/

  typedef struct {

    char SubChunk2Id[4];
    uint32_t SubChunk2Size;

  } dataField2;



  int main ()
  {
      setlocale(LC_ALL,"");   //configurando os caracteres da l�ngua portuguesa

      FILE *fp;               //ponteiro para arquivo
      FILE *nfp;              //new file pointer
      char namefile[50];      //armazena o nome do arquivo
      dataField header;       //criando uma vari�vel do tipo "dataField" para armazenar o cabe�alho
      dataField2 header2;
      int aux,i;
      uint16_t vol;            //volume
      char opcao;
      char string[4]="WAVE";
      uint32_t start, stop;



      /////////////////////////////////////// ABRINDO O ARQUIVO ///////////////////////////////////////
      printf("Digite o endere�o do arquivo WAVE que voc� deseja editar como no exemplo abaixo: \n");
      printf("Ex: C:\\�rea de Trabalho\\programa��o\\Exerc�cios\\Lista 1.PDF \n");
      printf("Utilize sempre DUAS barras entre cada endere�o e n�o esque�a da extens�o do arquivo !!\n");
      printf("Se o seu arquivo se encontra no mesmo diret�rio deste programa, apenas digite o seu nome e extens�o.\n\n");

      printf("Endere�o: ");
      gets(namefile);

      fp=fopen(namefile,"r+b");  //abrindo um arquivo bin�rio para leitura/escrita no endere�o indicado

      if(fp==NULL) {
        printf("\nO arquivo n�o existe.\n\n");
        return -1;
      }

      //////////////////// LENDO O CABE�ALHO DO ARQUIVO E ARMAZENANDO EM "HEADER" ////////////////////

     /*para esta parte do programa fora usada a fun��o "fread", que tem sua sintese explicada abaixo:

             fread (void *buffer, int numero_de_bytes, int count, FILE *fp);

      O buffer � a regi�o de mem�ria na qual ser�o armazenados os dados lidos. O n�mero de bytes �
      o tamanho da unidade a ser lida. count indica quantas unidades devem ser lidas. Isto significa
      que o n�mero total de bytes lidos �: numero_de_bytes*count

      informa��es encontradas em:
      https://terminalroot.com.br/2014/10/exemplos-de-funcoes-fread-fwrite-remove.html             */

      fread(&header,sizeof(header),1,fp);

      // a fun��o sizeof retorna o tamanho em bytes da estrutura, que nada mais � que a soma dos bytes
      // das vari�veis.

      //como a struct que armazenar� os dados do cabe�alho possui os mesmos tamanhos do cabe�alho wave
      //padr�o, os valores ir�o exatamente para posi��o que deveriam ir dentro da estrutura


      ///////////////////////////////// TESTANDO SE O ARQUIVO � WAVE //////////////////////////////////

      for(aux=0; aux<4; aux++)
      {
          if(header.format[aux]!=string[aux])
          {
               printf("\nEste n�o � um arquivo WAVE ! \n\n");
               return -2;
          }
      }

      /////////////////////////// VENDO ONDE EST�O OS META DADOS DO ARQUIVO ///////////////////////////
      /*
      OS metadados de um arquivo WAV indicam v�rias informa��es da m�sica, como por exemplo, nome do
      artista, nome da m�sica, da gravadora e etc...
      O problema destes dados � que eles n�o possuem uma localiza��o padr�o, os lugares mais comuns
      deles serem inseridos s�o estes:
      - depois do bytepersample
      - fim do arquivo
      E foi para estas duas poss�veis localiza��es que foi pensado o programa.

      Para definirmos onde estes metadados est�o, foi criada a fun��o "MetaDados"
      Seus possiveis retornos s�o:
      0 -> indica que os meta dados est�o no fim do arquivo
      tam. dos meta dados ->  indica que os meta dados est�o depois do bytepersample e ent�o retorna
      o tamanho dos metadados.
      */


      if(MetaDados(fp)==0)
        fseek(fp,sizeof(header),SEEK_SET);   //reposiciona o ponteiro para antes do "data" (SubChunk2Id)
      else
        fseek(fp,0,SEEK_CUR-4);              //posiciona o ponteiro depois dos metadados
        // SEEK_CUR -> posi��p atual do ponteiro


      /////////////////// PASSANDO O SUBCHUNK2ID E O SUBCHUNK2SIZE PARA O HEADER2 /////////////////////

      fread(&header2,sizeof(header2),1,fp);

      header2.SubChunk2Id[4]='\0';

      ////////////////////////////////////////// MENU  ///////////////////////////////////////////////

      do{
          system("cls");

          printf("Digite o que voc� gostaria de fazer com seu arquivo.\n\n");
          printf("1- Exibir dados t�cnicos do arquivo\n");
          printf("2- Alterar o volume do arquivo\n");
          printf("3- Recortar uma parte do arquivo\n\n");
          printf("Digite sua op��o: ");
          scanf("%d",&aux);


          switch(aux)
          {
           case 1:
                    printf("\nDados t�cnicos do arquivo selecionado \n\n");
                    printf("ChunkId: ");
                    PrintFourChar(header.ChunkId);
                    printf("\nChunkSize: %d\n",header.ChunkSize);
                    printf("Format: ");
                    PrintFourChar(header.format);
                    printf("\nSubChunkId: ");
                    PrintFourChar(header.SubChunkId);
                    printf("\nSubChunkSize: %d\n",header.SubChunkSize);
                    printf("AudioFormat: %d\n",header.AudioFormat);
                    printf("NumChannels: %d\n", header.NumChannels);
                    printf("SampleRate: %d\n",header.SampleRate);
                    printf("ByteRate: %d\n",header.ByteRate);
                    printf("BlockAlign: %d\n",header.BlockAlign);
                    printf("BitsPerSample: %d\n",header.BitsPerSample);
                    printf("SubChunk2Id: ");
                    PrintFourChar(header2.SubChunk2Id);
                    printf("\nSubChunk2Size: %d\n\n",header2.SubChunk2Size);

                    printf("Digite 'V' para voltar ao menu ou qualquer outra tecla para sair do programa:  ");
                    fflush(stdin);
                    scanf("%c",&opcao);
                    break;

          case 2:
                    printf("\nPara AUMENTAR o volume digite 1, para DIMINUIR o volume digite 0: ");
                    scanf("%d",&aux);

                    nfp=fopen("WAVE_mod.wav","wb");         //criando um arquivo bin�rio para escrita;
                    fwrite(&header,1,sizeof(header),nfp);   //passando o cabe�alho para o novo arquivo wave
                    fwrite(&header2,1,sizeof(header2),nfp); //passando os parametros ap�s o cabe�alho

                    if(aux)
                    {
                        printf("\nDigite um valor pelo qual a amplitude original do som ser� multiplicada: ");
                        scanf("%d",&aux);

                        while(!feof(fp))
                        {
                          fread(&vol,sizeof(vol),1,fp);    //passa o valor da amostra(16 bits) para a vari�vel volume(16 bits)
                          vol*=aux;                        //multiplica a vari�vel volume pelo valor que o usu�rio escolheu
                          fwrite(&vol,1,sizeof(vol),nfp);  //coloca essa vari�vel no arquivo novo
                        }

                    }
                    else
                    {
                        printf("\nDigite um valor pelo qual a amplitude original do som ser� dividida: ");
                        scanf("%d",&aux);

                        while(!feof(fp))
                        {
                          fread(&vol,sizeof(vol),1,fp);   //passa o valor da amostra(16 bits) para a vari�vel volume(16 bits)
                          vol/=aux;                       //divide a variavel volume pelo valor que o usu�rio escolheu
                          fwrite(&vol,1,sizeof(vol),nfp); //coloca essa vari�vel no arquivo novo
                        }

                    }

                    system("cls");

                    printf("O arquivo modificado foi colocado no mesmo diret�rio indicado para este!! seu nome � 'WAVE_mod'.  At� a pr�xima ! \n\n");

                    opcao='x';
                    break;

          case 3:

                    nfp=fopen("WAVE_mod.wav","wb");         //criando um arquivo bin�rio para escrita;
                    fwrite(&header,1,sizeof(header),nfp);   //passando o cabe�alho para o novo arquivo wave
                    fwrite(&header2,1,sizeof(header2),nfp); //passando os parametros ap�s o cabe�alho

                    printf("\nPara recortar um trecho do a�dio, digite o segundo inicial da faixa que voc� quer copiar e o segundo final. \n\n");
                    printf("Segundo inicial: ");
                    scanf("%d",&start);
                    printf("Segundo final: ");
                    scanf("%d",&stop);

                    start=header.SampleRate*start*2;  //(taxa de amostragem)*(segundo inicial)*(num. de bits por amostra /2)
                    stop =header.SampleRate*stop*2;   // (taxa de amostragem)*(segundo final)*(num. de bits por amostra /2)


                    fseek(fp,start,SEEK_CUR); //coloca o ponteiro na posi��o onde o usu�rio pediu
                    //SEEK_CUR -> posi��o atual do cursor
                    //a fun��o desloca o cursor at� (posi��o atual + valor de start)

                    aux=(stop-start)/2;      //diferen�a de bytes entre o start e o stop
                                             //dividido por dois, pois, ir� passar dois bytes por vez para o arquivo novo

                    for(i=0; i<aux; i++)
                    {
                         fread(&vol,sizeof(vol),1,fp);    //passa o valor de uma amostra(16 bits) para a vari�vel volume(16 bits)
                         fwrite(&vol,1,sizeof(vol),nfp);  //coloca essa vari�vel no arquivo novo

                         if(feof(fp))       //se o usu�rio digitou um tempo maior que o do audio e o arquivo chegou ao fim
                         {
                             opcao='x';
                             break;         //sai do while, garantindo que o ponteiro n�o saia do arquivo
                         }
                    }

                    // AJUSTANDO O CABE�ALHO //
                    aux*=2;                                                  //reajustando aux, para guardar o valor de bits do novo campo "data"
                    header2.SubChunk2Size=aux;                               //atribuindo ao subchunk2size (tamanho do campo de dados) o valor de aux
                    header.ChunkSize= sizeof(header)+sizeof(header2)+aux;    //atribuindo ao chunksize(valor total do arquivo) o valor do cabe�alho1 mais o cabe�alho 2 e mais o                     valor do data
                    rewind(nfp);                                             //recolocando o ponteiro no inicio do arquivo
                    fwrite(&header,1,sizeof(header),nfp);                    //sobrepondo o cabe�alho antigo pelo ajustado
                    fwrite(&header2,1,sizeof(header2),nfp);

                    system("cls");
                    printf("O recorte do arquivo foi colocado no mesmo diret�rio indicado para este!! seu nome � 'WAVE_mod'.  At� a pr�xima ! \n\n");

                    opcao='x';
                    break;

          default:
                    printf("\nOp��o inv�lida !\n\n");
                    break;
          }

      } while(opcao=='V' || opcao=='v');


      fclose(fp);
      fclose(nfp);

      system("pause");
      return 0;
  }


  void PrintFourChar(char *ch)
  {
      int j;

      for(j=0; j<4; j++)
        printf("%c",*(ch+j));
  }

  int MetaDados (FILE *file)
  {
      char aux[4]; aux[4]='\0';
      int flag=0,tam=0;

      while(flag==0)
      {
          fread(&aux,sizeof(aux),1,file);    //passa 4 bytes para a varia�vel aux

          if (strcmp(aux,"data")!=0)         //testa se esses bytes correspondem a "data"
            tam+=4;                          //se n�o corresponde, quer dizer que os metadados est�o aqui, portanto, come�amos a contar seu tamanho
          else                               //se corresponde
            flag=1;                          //os meta dados est�o no fim do arquivo, podemos sair do la�o
      }

      if(tam==0)        //metadados se encontra no fim do arquivo
          return 0;
      else              //meta dados no cabe�alho
          return tam;   //retorna o tamanho dos meta dados
  }
