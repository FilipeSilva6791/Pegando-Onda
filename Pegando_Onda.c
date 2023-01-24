 /********************************************************************
  @brief  Código para manipulação de arquivos de som no formato WAVE
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
   Créditos : Trecho de código inspirado no código do professor
      Marcos Zuccoloto escrito na aula de 25/11/20           */

  //esta struct armazenará os valores do cabeçalho
  //(o cabeçalho de um arquivo WAVE sempre é padrão)
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
      setlocale(LC_ALL,"");   //configurando os caracteres da língua portuguesa

      FILE *fp;               //ponteiro para arquivo
      FILE *nfp;              //new file pointer
      char namefile[50];      //armazena o nome do arquivo
      dataField header;       //criando uma variável do tipo "dataField" para armazenar o cabeçalho
      dataField2 header2;
      int aux,i;
      uint16_t vol;            //volume
      char opcao;
      char string[4]="WAVE";
      uint32_t start, stop;



      /////////////////////////////////////// ABRINDO O ARQUIVO ///////////////////////////////////////
      printf("Digite o endereço do arquivo WAVE que você deseja editar como no exemplo abaixo: \n");
      printf("Ex: C:\\Área de Trabalho\\programação\\Exercícios\\Lista 1.PDF \n");
      printf("Utilize sempre DUAS barras entre cada endereço e não esqueça da extensão do arquivo !!\n");
      printf("Se o seu arquivo se encontra no mesmo diretório deste programa, apenas digite o seu nome e extensão.\n\n");

      printf("Endereço: ");
      gets(namefile);

      fp=fopen(namefile,"r+b");  //abrindo um arquivo binário para leitura/escrita no endereço indicado

      if(fp==NULL) {
        printf("\nO arquivo não existe.\n\n");
        return -1;
      }

      //////////////////// LENDO O CABEÇALHO DO ARQUIVO E ARMAZENANDO EM "HEADER" ////////////////////

     /*para esta parte do programa fora usada a função "fread", que tem sua sintese explicada abaixo:

             fread (void *buffer, int numero_de_bytes, int count, FILE *fp);

      O buffer é a região de memória na qual serão armazenados os dados lidos. O número de bytes é
      o tamanho da unidade a ser lida. count indica quantas unidades devem ser lidas. Isto significa
      que o número total de bytes lidos é: numero_de_bytes*count

      informações encontradas em:
      https://terminalroot.com.br/2014/10/exemplos-de-funcoes-fread-fwrite-remove.html             */

      fread(&header,sizeof(header),1,fp);

      // a função sizeof retorna o tamanho em bytes da estrutura, que nada mais é que a soma dos bytes
      // das variáveis.

      //como a struct que armazenará os dados do cabeçalho possui os mesmos tamanhos do cabeçalho wave
      //padrão, os valores irão exatamente para posição que deveriam ir dentro da estrutura


      ///////////////////////////////// TESTANDO SE O ARQUIVO É WAVE //////////////////////////////////

      for(aux=0; aux<4; aux++)
      {
          if(header.format[aux]!=string[aux])
          {
               printf("\nEste não é um arquivo WAVE ! \n\n");
               return -2;
          }
      }

      /////////////////////////// VENDO ONDE ESTÃO OS META DADOS DO ARQUIVO ///////////////////////////
      /*
      OS metadados de um arquivo WAV indicam várias informações da música, como por exemplo, nome do
      artista, nome da música, da gravadora e etc...
      O problema destes dados é que eles não possuem uma localização padrão, os lugares mais comuns
      deles serem inseridos são estes:
      - depois do bytepersample
      - fim do arquivo
      E foi para estas duas possíveis localizações que foi pensado o programa.

      Para definirmos onde estes metadados estão, foi criada a função "MetaDados"
      Seus possiveis retornos são:
      0 -> indica que os meta dados estão no fim do arquivo
      tam. dos meta dados ->  indica que os meta dados estão depois do bytepersample e então retorna
      o tamanho dos metadados.
      */


      if(MetaDados(fp)==0)
        fseek(fp,sizeof(header),SEEK_SET);   //reposiciona o ponteiro para antes do "data" (SubChunk2Id)
      else
        fseek(fp,0,SEEK_CUR-4);              //posiciona o ponteiro depois dos metadados
        // SEEK_CUR -> posiçãp atual do ponteiro


      /////////////////// PASSANDO O SUBCHUNK2ID E O SUBCHUNK2SIZE PARA O HEADER2 /////////////////////

      fread(&header2,sizeof(header2),1,fp);

      header2.SubChunk2Id[4]='\0';

      ////////////////////////////////////////// MENU  ///////////////////////////////////////////////

      do{
          system("cls");

          printf("Digite o que você gostaria de fazer com seu arquivo.\n\n");
          printf("1- Exibir dados técnicos do arquivo\n");
          printf("2- Alterar o volume do arquivo\n");
          printf("3- Recortar uma parte do arquivo\n\n");
          printf("Digite sua opção: ");
          scanf("%d",&aux);


          switch(aux)
          {
           case 1:
                    printf("\nDados técnicos do arquivo selecionado \n\n");
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

                    nfp=fopen("WAVE_mod.wav","wb");         //criando um arquivo binário para escrita;
                    fwrite(&header,1,sizeof(header),nfp);   //passando o cabeçalho para o novo arquivo wave
                    fwrite(&header2,1,sizeof(header2),nfp); //passando os parametros após o cabeçalho

                    if(aux)
                    {
                        printf("\nDigite um valor pelo qual a amplitude original do som será multiplicada: ");
                        scanf("%d",&aux);

                        while(!feof(fp))
                        {
                          fread(&vol,sizeof(vol),1,fp);    //passa o valor da amostra(16 bits) para a variável volume(16 bits)
                          vol*=aux;                        //multiplica a variável volume pelo valor que o usuário escolheu
                          fwrite(&vol,1,sizeof(vol),nfp);  //coloca essa variável no arquivo novo
                        }

                    }
                    else
                    {
                        printf("\nDigite um valor pelo qual a amplitude original do som será dividida: ");
                        scanf("%d",&aux);

                        while(!feof(fp))
                        {
                          fread(&vol,sizeof(vol),1,fp);   //passa o valor da amostra(16 bits) para a variável volume(16 bits)
                          vol/=aux;                       //divide a variavel volume pelo valor que o usuário escolheu
                          fwrite(&vol,1,sizeof(vol),nfp); //coloca essa variável no arquivo novo
                        }

                    }

                    system("cls");

                    printf("O arquivo modificado foi colocado no mesmo diretório indicado para este!! seu nome é 'WAVE_mod'.  Até a próxima ! \n\n");

                    opcao='x';
                    break;

          case 3:

                    nfp=fopen("WAVE_mod.wav","wb");         //criando um arquivo binário para escrita;
                    fwrite(&header,1,sizeof(header),nfp);   //passando o cabeçalho para o novo arquivo wave
                    fwrite(&header2,1,sizeof(header2),nfp); //passando os parametros após o cabeçalho

                    printf("\nPara recortar um trecho do aúdio, digite o segundo inicial da faixa que você quer copiar e o segundo final. \n\n");
                    printf("Segundo inicial: ");
                    scanf("%d",&start);
                    printf("Segundo final: ");
                    scanf("%d",&stop);

                    start=header.SampleRate*start*2;  //(taxa de amostragem)*(segundo inicial)*(num. de bits por amostra /2)
                    stop =header.SampleRate*stop*2;   // (taxa de amostragem)*(segundo final)*(num. de bits por amostra /2)


                    fseek(fp,start,SEEK_CUR); //coloca o ponteiro na posição onde o usuário pediu
                    //SEEK_CUR -> posição atual do cursor
                    //a função desloca o cursor até (posição atual + valor de start)

                    aux=(stop-start)/2;      //diferença de bytes entre o start e o stop
                                             //dividido por dois, pois, irá passar dois bytes por vez para o arquivo novo

                    for(i=0; i<aux; i++)
                    {
                         fread(&vol,sizeof(vol),1,fp);    //passa o valor de uma amostra(16 bits) para a variável volume(16 bits)
                         fwrite(&vol,1,sizeof(vol),nfp);  //coloca essa variável no arquivo novo

                         if(feof(fp))       //se o usuário digitou um tempo maior que o do audio e o arquivo chegou ao fim
                         {
                             opcao='x';
                             break;         //sai do while, garantindo que o ponteiro não saia do arquivo
                         }
                    }

                    // AJUSTANDO O CABEÇALHO //
                    aux*=2;                                                  //reajustando aux, para guardar o valor de bits do novo campo "data"
                    header2.SubChunk2Size=aux;                               //atribuindo ao subchunk2size (tamanho do campo de dados) o valor de aux
                    header.ChunkSize= sizeof(header)+sizeof(header2)+aux;    //atribuindo ao chunksize(valor total do arquivo) o valor do cabeçalho1 mais o cabeçalho 2 e mais o                     valor do data
                    rewind(nfp);                                             //recolocando o ponteiro no inicio do arquivo
                    fwrite(&header,1,sizeof(header),nfp);                    //sobrepondo o cabeçalho antigo pelo ajustado
                    fwrite(&header2,1,sizeof(header2),nfp);

                    system("cls");
                    printf("O recorte do arquivo foi colocado no mesmo diretório indicado para este!! seu nome é 'WAVE_mod'.  Até a próxima ! \n\n");

                    opcao='x';
                    break;

          default:
                    printf("\nOpção inválida !\n\n");
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
          fread(&aux,sizeof(aux),1,file);    //passa 4 bytes para a variaável aux

          if (strcmp(aux,"data")!=0)         //testa se esses bytes correspondem a "data"
            tam+=4;                          //se não corresponde, quer dizer que os metadados estão aqui, portanto, começamos a contar seu tamanho
          else                               //se corresponde
            flag=1;                          //os meta dados estão no fim do arquivo, podemos sair do laço
      }

      if(tam==0)        //metadados se encontra no fim do arquivo
          return 0;
      else              //meta dados no cabeçalho
          return tam;   //retorna o tamanho dos meta dados
  }
