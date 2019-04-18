#include "ADParser.hpp"
#include <thread>
#include <chrono>

ADParser::ADParser(std::string inputFile, std::string engineType)
    : m_engineType(engineType), m_status(false), m_opened(false), m_current_step(-1)
{
    // TODO: append with rank, if necessary
    m_inputFile = inputFile;

    // int world_rank, world_size;
    // MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    // MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    // MPI_Comm adios_comm;
    // MPI_Group world_group;
    // MPI_Comm_group(MPI_COMM_WORLD, &world_group);
    // const int ranks[1] = {world_rank};
    // MPI_Group adios_group;
    // MPI_Group_incl(world_group, 1, ranks, &adios_group);
    // MPI_Comm_create_group(MPI_COMM_WORLD, adios_group, 0, &adios_comm);
    // MPI_Group_free(&world_group);
    // MPI_Group_free(&adios_group);
    // m_ad = adios2::ADIOS(adios_comm, adios2::DebugON);

    m_ad = adios2::ADIOS(MPI_COMM_SELF, adios2::DebugON);

    // set io and engine
    m_io = m_ad.DeclareIO("tau-metrics");
    m_io.SetEngine(m_engineType);
    //m_io.SetParameters();
    
    // open file
    // for sst engine, is the adios2 internally blocked here until *.sst file is found?
    m_reader = m_io.Open(m_inputFile, adios2::Mode::Read, MPI_COMM_SELF);

    m_opened = true;
    m_status = true;
}

ADParser::~ADParser() {
    if (m_opened) {
        m_reader.Close();
    }
}

int ADParser::beginStep() {
    if (m_opened)
    {
        const int max_tries = 10;
        int n_tries = 0;
        adios2::StepStatus status;
        while (n_tries < max_tries)
        {
            status = m_reader.BeginStep(adios2::StepMode::NextAvailable, 10.0f);
            if (status == adios2::StepStatus::NotReady)
            {
                std::this_thread::sleep_for(std::chrono::microseconds(1000));
                n_tries++;
                continue;
            }
            else if (status == adios2::StepStatus::OK)
            {
                m_current_step++;
                break;
            }
            else
            {
                m_status = false;
                return -1;
            }
        }
    }
    return m_current_step;
}

void ADParser::endStep() {
    if (m_opened) {
        m_reader.EndStep();
    }
}

void ADParser::update_attributes() {
    static bool once = false;

    if (!m_opened) return;
    if (m_engineType == "BPFile" && once) return;

    const std::map<std::string, adios2::Params> attributes = m_io.AvailableAttributes();
    for (const auto attributePair: attributes)
    {
        std::string name = attributePair.first;
        bool is_func = name.find("timer") != std::string::npos;
        bool is_event = name.find("event_type") != std::string::npos;

        if (!is_func && !is_event) continue;

        int key = std::stoi(name.substr(name.find(" ")));
        std::unordered_map<int, std::string>& m = (is_func) ? m_funcMap: m_eventType;
        if (m.count(key) == 0 && attributePair.second.count("Value"))
        {
            m[key] = attributePair.second.find("Value")->second;
        }
    }
    once = true;
}

ParserError ADParser::getFuncData() {
    adios2::Variable<size_t> in_timer_event_count;
    adios2::Variable<unsigned long> in_event_timestamps;
    size_t nelements;

    in_timer_event_count = m_io.InquireVariable<size_t>("timer_event_count");
    in_event_timestamps = m_io.InquireVariable<unsigned long>("event_timestamps");

    if (in_timer_event_count && in_event_timestamps)
    {
        m_reader.Get<size_t>(in_timer_event_count, &m_timer_event_count, adios2::Mode::Sync);

        nelements = m_timer_event_count * FUNC_EVENT_DIM; 
        if (nelements > m_event_timestamps.size())
            m_event_timestamps.resize(nelements);

        in_event_timestamps.SetSelection({{0, 0}, {m_timer_event_count, FUNC_EVENT_DIM}});
        m_reader.Get<unsigned long>(in_event_timestamps, m_event_timestamps.data(), adios2::Mode::Sync);
        return ParserError::OK;
    }
    return ParserError::NoFuncData;
}

ParserError ADParser::getCommData() {
    adios2::Variable<size_t> in_comm_count;
    adios2::Variable<unsigned long> in_comm_timestamps;
    size_t nelements;

    in_comm_count = m_io.InquireVariable<size_t>("comm_count");
    in_comm_timestamps = m_io.InquireVariable<unsigned long>("comm_timestamps");

    if (in_comm_count && in_comm_timestamps)
    {
        m_reader.Get<size_t>(in_comm_count, &m_comm_count, adios2::Mode::Sync);

        nelements = m_comm_count * COMM_EVENT_DIM;
        if (nelements > m_comm_timestamps.size())
            m_comm_timestamps.resize(nelements);

        in_comm_timestamps.SetSelection({{0, 0}, {m_comm_count, COMM_EVENT_DIM}});
        m_reader.Get<unsigned long>(in_comm_timestamps, m_comm_timestamps.data(), adios2::Mode::Sync);
        return ParserError::OK;
    }
    return ParserError::NoCommData;
}


template <typename K, typename V>
static void show_map(const std::unordered_map<K, V>& m)
{
    for (const auto& it : m)
    {
        std::cout << it.first << " : " << it.second << std::endl;
    }
}

void ADParser::show_funcMap() const {
    std::cout << "Function map: " << std::endl;
    show_map<int, std::string>(m_funcMap);
}

void ADParser::show_eventType() const {
    std::cout << "Event type map: " << std::endl;
    show_map<int, std::string>(m_eventType);
}