#ifndef KTELEPHONEMANAGER_H
#define KTELEPHONEMANAGER_H

#include <QHash>
#include <QSqlDatabase>

class KTelephone;
class KTelephoneGuide;

struct Telephone {
  QString id;
  QString description;
  QString name;
  QString domain;
  QString username;
  QString password;
  int active;
};

typedef struct Telephone Telephone_t;

class KTelephoneManager
{
 public:
    explicit KTelephoneManager();
    ~KTelephoneManager();

    void newKTelephone(Telephone_t);
    void updateKTelephone(Telephone_t);
    void saveTelephone(Telephone_t);
    void updateTelephone(Telephone_t);
    QHash<QString, KTelephone*> getTelephones();

 protected:
    QHash<QString, KTelephone*> telephones;
    KTelephone *mTelephone = NULL;
    KTelephoneGuide *mGuide = NULL;
    void startGuide();
    void connectDatabase();
    void bootstrapDatabase();
    void loadFromDatabase();
};

#endif // KTELEPHONEMANAGER_H
