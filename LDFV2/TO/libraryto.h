#ifndef LIBRARYTO_H
#define LIBRARYTO_H

#include "QObject"

///===============================================================================================///
/// \brief The LibratyTO class
/// Objeto de transferencia para serealizaçao dos dados
/// =============================================================================================///
class LibratyTO
{
 public:

        QList<QVector<float> >         descriptors;
        QVector<QImage>                imagem;
        QVector<QChar>                 character;
        QVector<QSize>                 size;

        friend QDataStream &operator >>(QDataStream &s,  LibratyTO& v)
        {
            s >> v.descriptors;
            s >> v.imagem;
            s >> v.character;
            s >> v.size;

            return s;
        }

        friend QDataStream &operator <<(QDataStream &s, const LibratyTO& v)
        {
            s << v.descriptors;
            s << v.imagem;
            s << v.character;
            s << v.size;

            return s;
        }

        friend QDebug operator << (QDebug dbg, const LibratyTO& v)
        {
            dbg.space() << "\nLoad library in recipe:\n";
            dbg.space() << v.descriptors;
            dbg.space() << v.imagem;
            dbg.space() << v.character;
            dbg.space() << v.size;

            return dbg.space();
        }

};
#endif // LIBRARYTO_H
