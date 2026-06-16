#include "Database.h"

Database::Database() : driver(nullptr), con(nullptr)
{
}

Database::~Database()
{
    Disconnect();
}

bool Database::Connect()
{
    try
    {
        driver = get_driver_instance();
        con = driver->connect(SERVER, USERNAME, PASSWORD);
        con->setSchema(DATABASE);

        std::cout << "Database connection done." << std::endl;
        return true;
    }
    catch (sql::SQLException& e)
    {
        std::cout << "Database could not connect to server. Error message: " << e.what() << std::endl;
        con = nullptr;
        return false;
    }
}

void Database::Disconnect()
{
    if (con == nullptr)
        return;

    con->close();

    if (con->isClosed())
    {
        std::cout << "Connection close." << std::endl;
        delete con;
        con = nullptr;
    }
}

bool Database::UserExists(const std::string& nickname)
{
    try
    {
        std::string query = "SELECT id FROM users WHERE nickname = ?";
        sql::PreparedStatement* stmt = con->prepareStatement(query);

        stmt->setString(1, nickname);

        sql::ResultSet* res = stmt->executeQuery();

        bool exists = res->next();

        delete res;
        delete stmt;

        return exists;
    }
    catch (sql::SQLException& e)
    {
        std::cout << "Error checking user: " << e.what() << std::endl;
        return false;
    }
}

bool Database::RegisterUser(const std::string& nickname, const std::string& password)
{
    try
    {
        if (UserExists(nickname))
        {
            std::cout << "Register failed: nickname already exists." << std::endl;
            return false;
        }

        // Insert user
        std::string insertUserQuery = "INSERT INTO users (nickname, password_hash) VALUES (?, ?)";
        sql::PreparedStatement* stmtUser = con->prepareStatement(insertUserQuery);

        stmtUser->setString(1, nickname);
        stmtUser->setString(2, password);

        int affectedRows = stmtUser->executeUpdate();
        delete stmtUser;

        if (affectedRows <= 0)
        {
            std::cout << "Register failed: user insert failed." << std::endl;
            return false;
        }

        // Get last inserted ID
        sql::PreparedStatement* stmtId = con->prepareStatement("SELECT LAST_INSERT_ID() AS user_id");
        sql::ResultSet* resId = stmtId->executeQuery();

        int userId = -1;

        if (resId->next())
        {
            userId = resId->getInt("user_id");
        }

        delete resId;
        delete stmtId;

        if (userId == -1)
        {
            std::cout << "Register failed: could not retrieve ID." << std::endl;
            return false;
        }

        // Insert ranking
        std::string insertRankingQuery = "INSERT INTO rankings (user_id, nickname, points, wins, losses, draws) VALUES (?, ?, ?, ?, ?, ?)";
        sql::PreparedStatement* stmtRanking = con->prepareStatement(insertRankingQuery);

        stmtRanking->setInt(1, userId);
        stmtRanking->setString(2, nickname);
        stmtRanking->setInt(3, newUserPoints);
        stmtRanking->setInt(4, 0);
        stmtRanking->setInt(5, 0);
        stmtRanking->setInt(6, 0);

        int rankingRows = stmtRanking->executeUpdate();
        delete stmtRanking;

        if (rankingRows <= 0)
        {
            std::cout << "Register failed: ranking insert failed." << std::endl;
            return false;
        }

        std::cout << "User registered successfully. ID: " << userId << std::endl;
        return true;
    }
    catch (sql::SQLException& e)
    {
        std::cout << "Error registering user: " << e.what() << std::endl;
        return false;
    }
}

bool Database::LoginUser(const std::string& nickname, const std::string& password, int& userId)
{
    userId = -1;

    try
    {
        std::string query = "SELECT id, password_hash FROM users WHERE nickname = ?";
        sql::PreparedStatement* stmt = con->prepareStatement(query);

        stmt->setString(1, nickname);

        sql::ResultSet* res = stmt->executeQuery();

        if (!res->next())
        {
            std::cout << "Login failed: user not found." << std::endl;
            delete res;
            delete stmt;
            return false;
        }

        int dbUserId = res->getInt("id");
        std::string dbPassword = res->getString("password_hash");

        delete res;
        delete stmt;

        if (!bcrypt::validatePassword(password, dbPassword))
        {
            std::cout << "Login failed: wrong password." << std::endl;
            return false;
        }

        userId = dbUserId;

        std::cout << "Login successful. ID: " << userId << std::endl;
        return true;
    }
    catch (sql::SQLException& e)
    {
        std::cout << "Error logging in: " << e.what() << std::endl;
        return false;
    }
}

std::vector<Database::RankingEntry> Database::GetTop10Rankings(int userId)
{
    std::vector<RankingEntry> top10;

    try
    {
        std::string query = "SELECT user_id, points, nickname FROM rankings ORDER BY points DESC LIMIT 10";

        sql::PreparedStatement* stmt = con->prepareStatement(query);
        sql::ResultSet* res = stmt->executeQuery();

        int position = 1;
        while (res->next() && position <= 10)
        {
            RankingEntry entry;
            entry.position = position;
            entry.userId = res->getInt("user_id");
            entry.points = res->getInt("points");
            entry.username = res->getString("nickname");

            top10.push_back(entry);
            position++;
        }

        delete res;
        delete stmt;

        std::cout << "Recibido " << top10.size() << " puntuaciones." << std::endl;
    }
    catch (sql::SQLException& e)
    {
        std::cout << "Error consiguiendo top 10 rankings: " << e.what() << std::endl;
    }

    return top10;
}
