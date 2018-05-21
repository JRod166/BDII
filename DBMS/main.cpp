#include <iostream>
#include <regex>
#include <string>
#include <conio.h>
#include <vector>
#include <fstream>

using namespace std;
regex create("create table ([a-zA-Z]|\\d)+ \\( (([a-zA-Z]|\\d)+ (int|char|date) \\d+, )*([a-zA-Z]|\\d)+ (int|char|date) \\d+ \\)");
regex select("select \\* from ([a-zA-Z]|\\d)+ where ([a-zA-Z]|\\d)+ (\\=|\\>|\\<) ([a-zA-Z]|\\d|\\-)+");
//regex select("select ((\\*)|([a-zA-Z]|\\d)+) from ([a-zA-Z]|\\d)+ where ([a-zA-Z]|\\d)+ (\\=|\\>|\\<) \\d+");
regex inserta("insert into ([a-zA-Z]|\\d)+ \\( (([a-zA-Z]|\\d)+, )*([a-zA-Z]|\\d)+ \\) values \\( (([a-zA-Z]|\\d|\\-)+, )*([a-zA-Z]|\\d|\\-)+ \\)");
regex deletea("delete from ([a-zA-Z]|\\d)+ where ([a-zA-Z]|\\d)+ (\\=|\\>|\\<) ([a-zA-Z]|\\d|\\-)+");
regex update("update from ([a-zA-Z]|\\d)+ set (([a-zA-Z]|\\d)+ \\= ([a-zA-Z]|\\d)+, )*([a-zA-Z]|\\d)+ \\= ([a-zA-Z]|\\d)+ where ([a-zA-Z]|\\d)+ (\\=|\\>|\\<) ([a-zA-Z]|\\d|\\-)+");
regex cant("\\( ([a-zA-Z]|\\d)+ \\d+ \\)");
regex help("(help|h)");
regex salir("exit");

bool get_meta(vector<int> *tam,vector<string> *columnas_tabla,vector<string> *tipos,fstream *metadata)
{
    int k;
    string aux2,aux3;
    while (getline(*metadata,aux2))
    {
        k=0;
        while(aux2[k]!=' ')
        {
            aux3+=aux2[k];
            k++;
        }
        k++;
        columnas_tabla->push_back(aux3);
        aux3.clear();
        while(aux2[k]!=' ')
        {
            aux3+=aux2[k];
            k++;
        }
        k++;
        tipos->push_back(aux3);
        aux3.clear();
        while(k<=aux2.size())
        {
            aux3+=aux2[k];
            k++;
        }
        tam->push_back(stoi(aux3));
        aux3.clear();
    }

}

bool create_table(string a)
{
    int string_sz=a.size()-14,i;
    string table_name,aux,aux2;
    vector<string> columnas, atributos,tamanos;


    aux=a.substr(13,string_sz);
    while(aux[i]!=' ')
    {
        table_name+=aux[i];
        i++;
    }
    i+=3;
    while(i<string_sz)
    {
        while(aux[i]!=' ')
        {
            aux2+=aux[i];
            i++;
        }
        columnas.push_back(aux2);
        i++; aux2.clear();
        while(aux[i]!=' ')
        {
            aux2+=aux[i];
            i++;
        }
        atributos.push_back(aux2);
        i++; aux2.clear();
        while(aux[i]!=' ' && aux[i]!=',')
        {
            aux2+=aux[i];
            i++;
        }
        tamanos.push_back(aux2);
        i+=2; aux2.clear();

    }
    aux="./metadata/"+table_name+".meta";
    aux2="./tablas/"+table_name+".tab";
    fstream metadata(aux,fstream::out);
    fstream tabla(aux2,fstream::out);

    for (int j=0;j<columnas.size();j++)
    {
        metadata<<columnas[j]<<" "<<atributos[j]<<" "<<tamanos[j]<<endl;
    }

    return 1;
}

bool insert_into(string a)
{
    string table_name,aux,aux2,aux3;
    int i=0,cont, k;
    int string_sz=a.size()-12;
    vector <string> columnas,valores,columnas_tabla,tipos,resultados;
    vector <int> tam;
    fstream metadata;
    char c;

    aux=a.substr(12,string_sz);
    while(aux[i]!=' ')
    {
        table_name+=aux[i];
        i++;
    }
    i+=3;
    while(aux[i]!=')')
    {
        if (aux[i]==',' || aux[i]==' ')
        {
            if(aux2!=""){
                columnas.push_back(aux2);
                aux2.clear();
            }
            i++;
        }
        else
        {
            aux2+=aux[i];
            i++;
        }
    }
    i+=11;
    while(aux[i]!=')')
    {
        if (aux[i]==',' || aux[i]==' ')
        {
            if(aux2!=""){
                valores.push_back(aux2);
                aux2.clear();
            }
            i++;
        }
        else
        {
            aux2+=aux[i];
            i++;
        }
    }
    if(columnas.size()!=valores.size()) {return 0;}

    aux="./metadata/"+table_name+".meta";
    metadata.open(aux,std::fstream::in | std::fstream::out);
    get_meta(&tam,&columnas_tabla,&tipos,&metadata);
    metadata.close();

    for(int p=0;p<columnas_tabla.size();p++)
    {
        resultados.push_back(" ");
        for(int q=0;q<columnas.size();q++)
        {
            if (columnas[q]==columnas_tabla[p])
            {
                resultados[p]=valores[q];
            }
        }
    }
    int temp;
    for (int p=0;p<resultados.size();p++)
    {
        temp=tam[p]-resultados[p].size();
        for(int q=0;q<temp;q++)
        {
            resultados[p]=' '+resultados[p];
        }
    }
    aux3.clear();
    for (int p=0;p<resultados.size();p++)
    {
        aux3+=resultados[p]+',';
    }
    aux3[aux3.size()-1]='/';

    aux="./tablas/"+table_name+".tab";
    metadata.open(aux,std::fstream::in | std::fstream::out);
    metadata.seekg(0,metadata.end);
    cont=metadata.tellg();
    metadata.seekp(cont);
    metadata<<aux3;
    metadata.flush();
    metadata.close();
    return 1;

}

bool bloque(string a)
{
    string table_name,aux,aux2,aux3;
    int i=0,cont, k,cantidad;
    int string_sz=a.size()-12;
    vector <string> columnas,valores,columnas_tabla,tipos,resultados;
    vector <int> tam;
    fstream metadata;
    char c;

    aux=a.substr(2,a.size()-3);
    while(aux[i]!=' ')
    {
        table_name+=aux[i];
        i++;
    }
    i++;
    while(aux[i]!=' ')
    {
        aux2+=aux[i];
        i++;
    }
    cantidad=stoi(aux2);
    aux="./metadata/"+table_name+".meta";
    metadata.open(aux,std::fstream::in | std::fstream::out);
    get_meta(&tam,&columnas_tabla,&tipos,&metadata);
    metadata.close();
    cout<<"insertando"<<endl;
    aux="./tablas/"+table_name+".tab";
    metadata.open(aux,std::fstream::in | std::fstream::out);

    for (int it=0; it<cantidad;it++)
    {
        cout<<"insercion "<<it+1<<" de "<<cantidad<<endl;
        resultados.push_back(to_string(it));
        resultados.push_back("nombre"+to_string(it));
        resultados.push_back("apellido"+to_string(it));
        resultados.push_back(to_string(rand()%100));
        resultados.push_back(to_string((rand()%30)+1)+"-"+to_string((rand()%12)+1)+"-"+to_string((rand()%28)+1990));
        int temp;
        for (int p=0;p<resultados.size();p++)
        {
            temp=tam[p]-resultados[p].size();
            for(int q=0;q<temp;q++)
            {
                resultados[p]=' '+resultados[p];
            }
        }
        aux3.clear();
        for (int p=0;p<resultados.size();p++)
        {
            aux3+=resultados[p]+',';
        }
        aux3[aux3.size()-1]='/';
        metadata.seekg(0,metadata.end);
        cont=metadata.tellg();
        metadata.seekp(cont);
        metadata<<aux3;
        metadata.flush();
        resultados.clear();
    }
        metadata.close();
    return 1;

}

vector <string> selectiona(string a)
{
    vector<int> tam;
    vector<string> columnas_tabla, tipos,respuesta;
    fstream archivo;
    int string_sz=a.size()-14;
    string aux,aux2,aux3,aux4,table_name,columna_cond,cond,condicionada;
    aux=a.substr(14,string_sz);
    int i=0,tam_reg,aux_tam,index_cond,auxiliar,auxiliar2;
    while(aux[i]!=' ')
    {
        table_name+=aux[i];
        i++;
    }
    i++;
    i+=6;
    while(aux[i]!=' ')
    {
        columna_cond+=aux[i];
        i++;
    }
    i++;
    cond=aux[i];
    i++;
    while(i<string_sz)
    {
        aux2+=aux[i];
        i++;
    }
    condicionada=aux2;
    aux="./metadata/"+table_name+".meta";
    archivo.open(aux,std::fstream::in | std::fstream::out);
    get_meta(&tam,&columnas_tabla,&tipos,&archivo);
    archivo.close();
    tam_reg=0,aux_tam=0;
    for (int j=0;j<tam.size();j++)
    {
        if(columnas_tabla[j]==columna_cond)
        {
            index_cond=j;
        }
        tam_reg+=tam[j];
        tam_reg++;
    }
    int j=0;
    while (columnas_tabla[j]!=columna_cond)
    {
        aux_tam+=tam[j];
        aux_tam++;
        j++;
    }
    aux2.clear();
    aux="./tablas/"+table_name+".tab";
    archivo.open(aux,std::fstream::in | std::fstream::out);
    archivo.seekg(0,archivo.end);
    int length=archivo.tellg();
    archivo.seekg(0,archivo.beg);
    char buffer[tam_reg-1];
    auxiliar=0;
    int temp;
    do
    {
        archivo.read(buffer,tam_reg-1);
        aux2.clear();
        for(int i=0;i<tam_reg;i++)
        {
            aux2+=buffer[i];
        }
        aux3=aux2.substr(aux_tam,tam[index_cond]);
        temp=tam[j]-condicionada.size();
        for(int q=0;q<temp;q++)
        {
            condicionada=' '+condicionada;
        }
        if (cond=="=")
        {
            if(condicionada==aux3)
            {
                respuesta.push_back(aux2);
            }
        }
        else
        {
            char buffer_vacio[aux3.size()];
            for (int alpaca=0;alpaca<aux3.size();alpaca++)
            {
                buffer_vacio[alpaca]=' ';
            }
            if (aux3==buffer_vacio)
            {

            }
            else if (tipos[index_cond]=="date")
            {
                int i=0;
                while(aux3[i]!='-')
                {
                    aux4+=aux3[i];
                    i++;
                }
                i++;
                auxiliar2=stoi(aux4);
                aux4.clear();
                while(aux3[i]!='-')
                {
                    aux4+=aux3[i];
                    i++;
                }
                auxiliar2+=(stoi(aux4)*30);
                if(cond=="<")
                {
                    if (stoi(condicionada.substr(tam[index_cond]-3,tam[index_cond]))>=stoi(aux3.substr(tam[index_cond]-4,tam[index_cond])))
                    {
                        respuesta.push_back(aux2);
                    }
                    else
                    {
                        if(auxiliar2<stoi(condicionada.substr(0,2))+(stoi(condicionada.substr(3,5))*30))
                        {
                            respuesta.push_back(aux2);
                        }
                    }
                }
                if ( cond ==">")
                {
                    if (stoi(condicionada.substr(tam[index_cond]-3,tam[index_cond]))<=stoi(aux3.substr(tam[index_cond]-4,tam[index_cond])))
                    {
                        respuesta.push_back(aux2);
                    }
                    else
                    {
                        if(auxiliar2>stoi(condicionada.substr(0,2))+(stoi(condicionada.substr(3,5))*30))
                        {
                            respuesta.push_back(aux2);
                        }
                    }
                }
            }
            else if(tipos[index_cond]=="int")
            {
                if(cond=="<")
                {
                    if(stoi(aux3)<stoi(condicionada))
                    {
                        respuesta.push_back(aux2);
                    }
                }
                if(cond==">")
                {
                    if(stoi(aux3)>stoi(condicionada))
                    {
                        respuesta.push_back(aux2);
                    }
                }
            }
            else{
                cout<<"No es posible aplicar el operador logico "<<cond<<" en este tipo de dato"<<endl;
                return respuesta;
            }
        }
        auxiliar+=tam_reg;
        archivo.seekg(auxiliar);
    } while (auxiliar<length);

    return respuesta;



}

void deletear(string a)
{
    vector<int> tam;
    vector<string> columnas_tabla, tipos,respuesta;
    fstream archivo;
    int string_sz=a.size();
    string aux,aux2,aux3,aux4,table_name,columna_cond,cond,condicionada;
    aux=a.substr(14,string_sz);
    int i=0,tam_reg,aux_tam,index_cond,auxiliar,auxiliar2;

    aux=a.substr(12, string_sz);
    while(aux[i]!=' ')
    {
        table_name+=aux[i];
        i++;
    }
    i++;
    i+=6;
    while(aux[i]!=' ')
    {
        columna_cond+=aux[i];
        i++;
    }
    i++;
    cond=aux[i];
    i++;
    while(i<string_sz-12)
    {
        aux2+=aux[i];
        i++;
    }
    condicionada=aux2;
    aux="./metadata/"+table_name+".meta";
    archivo.open(aux,std::fstream::in | std::fstream::out);
    get_meta(&tam,&columnas_tabla,&tipos,&archivo);
    archivo.close();
    tam_reg=0,aux_tam=0;
    for (int j=0;j<tam.size();j++)
    {
        if(columnas_tabla[j]==columna_cond)
        {
            index_cond=j;
        }
        tam_reg+=tam[j];
        tam_reg++;
    }
    int j=0;
    while (columnas_tabla[j]!=columna_cond)
    {
        aux_tam+=tam[j];
        aux_tam++;
        j++;
    }
    aux2.clear();
    aux="./tablas/"+table_name+".tab";
    archivo.open(aux,std::fstream::in | std::fstream::out);
    archivo.seekg(0,archivo.end);
    int length=archivo.tellg();
    archivo.seekg(0,archivo.beg);
    char buffer[tam_reg-1],buffer_vacio[tam_reg-1];
    for (int alpaca=0;alpaca<tam_reg-1;alpaca++)
    {
        buffer_vacio[alpaca]=' ';
    }
    auxiliar=0;
    int temp;
    do
    {
        archivo.read(buffer,tam_reg-1);
        aux2=buffer;
        aux3=aux2.substr(aux_tam,tam[index_cond]);
        temp=tam[j]-condicionada.size();
        for(int q=0;q<temp;q++)
        {
            condicionada=' '+condicionada;
        }
        if (cond=="=")
        {
            if(condicionada==aux3)
            {
                archivo.seekp(auxiliar);
                archivo.write(buffer_vacio,tam_reg-1);
                archivo.flush();
            }
        }
        else
        {
            char buffer_vacio[aux3.size()];
            for (int alpaca=0;alpaca<aux3.size();alpaca++)
            {
                buffer_vacio[alpaca]=' ';
            }
            if (aux3==buffer_vacio)
            {

            }
            else if (tipos[index_cond]=="date")
            {
                int i=0;
                while(aux3[i]!='-')
                {
                    aux4+=aux3[i];
                    i++;
                }
                i++;
                auxiliar2=stoi(aux4);
                aux4.clear();
                while(aux3[i]!='-')
                {
                    aux4+=aux3[i];
                    i++;
                }
                auxiliar2+=(stoi(aux4)*30);
                if(cond=="<")
                {
                    if (stoi(condicionada.substr(tam[index_cond]-3,tam[index_cond]))>=stoi(aux3.substr(tam[index_cond]-4,tam[index_cond])))
                    {
                        archivo.seekp(auxiliar);
                        archivo.write(buffer_vacio,tam_reg-1);
                        archivo.flush();
                    }
                    else
                    {
                        if(auxiliar2<stoi(condicionada.substr(0,2))+(stoi(condicionada.substr(3,5))*30))
                        {
                            archivo.seekp(auxiliar);
                            archivo.write(buffer_vacio,tam_reg-1);
                            archivo.flush();
                        }
                    }
                }
                if ( cond ==">")
                {
                    if (stoi(condicionada.substr(tam[index_cond]-3,tam[index_cond]))<=stoi(aux3.substr(tam[index_cond]-4,tam[index_cond])))
                    {
                        archivo.seekp(auxiliar);
                        archivo.write(buffer_vacio,tam_reg-1);
                        archivo.flush();
                    }
                    else
                    {
                        if(auxiliar2>stoi(condicionada.substr(0,2))+(stoi(condicionada.substr(3,5))*30))
                        {
                            archivo.seekp(auxiliar);
                            archivo.write(buffer_vacio,tam_reg-1);
                            archivo.flush();
                        }
                    }
                }
            }
            else if(tipos[index_cond]=="int")
            {
                if(cond=="<")
                {
                    if(stoi(aux3)<stoi(condicionada))
                    {
                        archivo.seekp(auxiliar);
                        archivo.write(buffer_vacio,tam_reg-1);
                        archivo.flush();
                    }
                }
                if(cond==">")
                {
                    if(stoi(aux3)>stoi(condicionada))
                    {
                        archivo.seekp(auxiliar);
                        archivo.write(buffer_vacio,tam_reg-1);
                        archivo.flush();
                    }
                }
            }
            else{
                cout<<"No es posible aplicar el operador logico "<<cond<<" en este tipo de dato"<<endl;
                return ;
            }
        }
        auxiliar+=tam_reg;
        archivo.seekg(auxiliar);
    } while (auxiliar<length);

    return ;
}

int updatea(string a)
{
    vector<int> tam,tam_agregar;
    vector<string> columnas_tabla, tipos,respuesta,columnas,resultados;
    fstream archivo;
    int string_sz=a.size(),contador=0;
    string aux,aux2,aux3,aux4,table_name,columna_cond,cond,condicionada;
    int i=0,tam_reg,aux_tam,index_cond,auxiliar,auxiliar2;

    aux=a.substr(12,string_sz);
    while(aux[i]!=' ')
    {
        table_name+=aux[i];
        i++;
    }
    i+=5;
    aux.clear();
    vector<string> temporal;
    while (i < string_sz-12)
    {
        while(aux[i]!=',' and aux[i]!=' ' and i<string_sz-12)
        {
            aux2+=aux[i];
            i++;
        }
        if (aux2!="")
        {
            temporal.push_back(aux2);
        }
        aux2.clear();
        i++;
    }i=0;
    while(temporal[i]!="where")
    {
        if(i%3==0)
        {
            columnas.push_back(temporal[i]);
        }
        if(i%3==2)
        {
            resultados.push_back(temporal[i]);
        }
        i++;
    }
    vector <int> tam_mod;
    columna_cond=temporal[temporal.size()-3];
    cond=temporal[temporal.size()-2];
    condicionada=temporal[temporal.size()-1];
    aux="./metadata/"+table_name+".meta";
    archivo.open(aux,std::fstream::in | std::fstream::out);
    get_meta(&tam,&columnas_tabla,&tipos,&archivo);
    archivo.close();
    tam_reg=0,aux_tam=0;
    for (int j=0;j<tam.size();j++)
    {
        if(columnas_tabla[j]==columna_cond)
        {
            index_cond=j;
        }
        tam_reg+=tam[j];
        tam_reg++;
    }
    int apoyo=0;
    for(int j=0;j<columnas.size();j++)
    {
        tam_mod.push_back(0);
        apoyo=0;
        for(int k=0;k<columnas_tabla.size();k++)
        {
            if(columnas_tabla[k]==columnas[j])
            {
                tam_mod[j]=apoyo;
                tam_agregar.push_back(tam[k]);
            }
            apoyo+=tam[k];
            apoyo++;
        }
    }
    int temp;
    for (int p=0;p<resultados.size();p++)
    {
        temp=tam_agregar[p]-resultados[p].size();
        for(int q=0;q<temp;q++)
        {
            resultados[p]=' '+resultados[p];
        }
    }
///update from 10mil set nombre = pancho, apellido = ricachon where id = 58
    int j=0;
    while (columnas_tabla[j]!=columna_cond)
    {
        aux_tam+=tam[j];
        aux_tam++;
        j++;
    }
    aux2.clear();
    aux="./tablas/"+table_name+".tab";
    archivo.open(aux,std::fstream::in | std::fstream::out);
    archivo.seekg(0,archivo.end);
    int length=archivo.tellg();
    archivo.seekg(0,archivo.beg);
    char buffer[tam_reg-1];
    auxiliar=0;
    do
    {
        archivo.read(buffer,tam_reg-1);
        aux2=buffer;
        aux3=aux2.substr(aux_tam,tam[index_cond]);
        temp=tam[j]-condicionada.size();
        for(int q=0;q<temp;q++)
        {
            condicionada=' '+condicionada;
        }
        if (cond=="=")
        {
            if(condicionada==aux3)
            {
                for (int r=0;r<resultados.size();r++)
                {
                    archivo.seekg(auxiliar+tam_mod[r]);
                    char buffertemp[tam_agregar[r]];

                    for(int sd=0;sd<resultados[r].size();sd++)
                    {
                        buffertemp[sd]=resultados[r][sd];
                    }
                    archivo.write(buffertemp,resultados[r].size());
                    archivo.flush();
                }
                contador++;
            }
        }
        else
        {
            char buffer_vacio[aux3.size()];
            for (int alpaca=0;alpaca<aux3.size();alpaca++)
            {
                buffer_vacio[alpaca]=' ';
            }
            if (aux3==buffer_vacio)
            {

            }
            else if (tipos[index_cond]=="date")
            {
                int i=0;
                while(aux3[i]!='-')
                {
                    aux4+=aux3[i];
                    i++;
                }
                i++;
                auxiliar2=stoi(aux4);
                aux4.clear();
                while(aux3[i]!='-')
                {
                    aux4+=aux3[i];
                    i++;
                }
                auxiliar2+=(stoi(aux4)*30);
                if(cond=="<")
                {
                    if (stoi(condicionada.substr(tam[index_cond]-3,tam[index_cond]))>=stoi(aux3.substr(tam[index_cond]-4,tam[index_cond])))
                    {
                        for (int r=0;r<resultados.size();r++)
                        {
                            archivo.seekg(auxiliar+tam_mod[r]);
                            char buffertemp[tam_agregar[r]];

                            for(int sd=0;sd<resultados[r].size();sd++)
                            {
                                buffertemp[sd]=resultados[r][sd];
                            }
                            archivo.write(buffertemp,resultados[r].size());
                            archivo.flush();
                        }
                        contador++;
                    }
                    else
                    {
                        if(auxiliar2<stoi(condicionada.substr(0,2))+(stoi(condicionada.substr(3,5))*30))
                        {
                            for (int r=0;r<resultados.size();r++)
                            {
                                archivo.seekg(auxiliar+tam_mod[r]);
                                char buffertemp[tam_agregar[r]];

                                for(int sd=0;sd<resultados[r].size();sd++)
                                {
                                    buffertemp[sd]=resultados[r][sd];
                                }
                                archivo.write(buffertemp,resultados[r].size());
                                archivo.flush();
                            }
                            contador++;
                        }
                    }
                }
                if ( cond ==">")
                {
                    if (stoi(condicionada.substr(tam[index_cond]-3,tam[index_cond]))<=stoi(aux3.substr(tam[index_cond]-4,tam[index_cond])))
                    {
                        for (int r=0;r<resultados.size();r++)
                        {
                            archivo.seekg(auxiliar+tam_mod[r]);
                            char buffertemp[tam_agregar[r]];

                            for(int sd=0;sd<resultados[r].size();sd++)
                            {
                                buffertemp[sd]=resultados[r][sd];
                            }
                            archivo.write(buffertemp,resultados[r].size());
                            archivo.flush();
                        }
                        contador++;
                    }
                    else
                    {
                        if(auxiliar2>stoi(condicionada.substr(0,2))+(stoi(condicionada.substr(3,5))*30))
                        {
                            for (int r=0;r<resultados.size();r++)
                            {
                                archivo.seekg(auxiliar+tam_mod[r]);
                                char buffertemp[tam_agregar[r]];

                                for(int sd=0;sd<resultados[r].size();sd++)
                                {
                                    buffertemp[sd]=resultados[r][sd];
                                }
                                archivo.write(buffertemp,resultados[r].size());
                                archivo.flush();
                            }
                            contador++;
                        }
                    }
                }
            }
            else if(tipos[index_cond]=="int")
            {
                if(cond=="<")
                {
                    if(stoi(aux3)<stoi(condicionada))
                    {
                        for (int r=0;r<resultados.size();r++)
                        {
                            archivo.seekg(auxiliar+tam_mod[r]);
                            char buffertemp[tam_agregar[r]];

                            for(int sd=0;sd<resultados[r].size();sd++)
                            {
                                buffertemp[sd]=resultados[r][sd];
                            }
                            archivo.write(buffertemp,resultados[r].size());
                            archivo.flush();
                        }
                        contador++;
                    }
                }
                if(cond==">")
                {
                    if(stoi(aux3)>stoi(condicionada))
                    {
                        for (int r=0;r<resultados.size();r++)
                        {
                            archivo.seekg(auxiliar+tam_mod[r]);
                            char buffertemp[tam_agregar[r]];

                            for(int sd=0;sd<resultados[r].size();sd++)
                            {
                                buffertemp[sd]=resultados[r][sd];
                            }
                            archivo.write(buffertemp,resultados[r].size());
                            archivo.flush();
                        }
                        contador++;
                    }
                }
            }
            else{
                cout<<"No es posible aplicar el operador logico "<<cond<<" en este tipo de dato"<<endl;
                return 0;
            }
        }
        auxiliar+=tam_reg;
        archivo.seekg(auxiliar);
    } while (auxiliar<length);

    ///UPDATE FROM table_name SET atributo=valor....
    return contador;
}

int main()
{
    while (true)
    {
        string a;
        getline(cin,a);
        if (regex_match(a,create))
        {
            if(create_table(a))
                cout<<"Tabla creada exitosamente"<<endl;
        }
        else if (regex_match(a,select))
        {
            vector <string> seleccion;
            seleccion=selectiona(a);
            for (int i=0;i<seleccion.size();i++)
            {
                cout<<seleccion[i]<<endl;
            }
        }
        else if (regex_match(a,inserta))
        {
            if(insert_into(a))
            {
                cout<<"Inserción exitosa"<<endl;
            }
            else
            {
                cout<<"Error"<<endl;
            }
        }
        else if (regex_match(a,deletea))
        {
            deletear(a);
            cout<<"Borrado terminado"<<endl;
        }
        else if (regex_match(a,update))
        {
            int cantidad;
            cout<<"update"<<endl;
            cantidad=updatea(a);
            cout<<cantidad<<" filas modificadas"<<endl;
        }
        else if (regex_match(a,cant))
        {
            bloque(a);
        }
        else if (regex_match(a,help))
        {
            cout<<"create table table_name ( col_name type size, col_name type size, ... )"<<endl;
            cout<<"select * from table_name where col_name =/</> value"<<endl;
            cout<<"insert into table_name (col_name, col_name, ... ) values ( value, value, ... )"<<endl;
            cout<<"delete from table_name where col_name =/</> value"<<endl;
            cout<<"update from table_name set col_name = value, col_name = value, ... where col_name =/</> value"<<endl;
            cout<<"date format: DD-MM-YYYY"<<endl;
        }
        else if(regex_match(a,salir))
        {
            return 0;
        }
        else
        {
            cout<<"expresion no valida"<<endl;
        }
    }
    cout << "Hello world!" << endl;
    return 0;
}
