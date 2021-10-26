//
// Created by Carla on 24/06/2021.
//

#include <iostream>
#include <composer/melody.h>
#include <pareto/archive.h>
#include "MidiFile.h"
#include "Options.h"
#include "matplot/matplot.h"
#include <pareto/matplot/archive.h>
#include <functional>
#include <fstream>

using namespace composer;

void nsga_ii(int execution) {

    std::ostringstream stream;
    stream << "experiment_" << execution << ".txt";
    std::string experiment = stream.str();

    std::ofstream file(experiment);

    file << "Execution " << execution << " ";

    static std::default_random_engine generator_;
    generator_ = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

    // Problem parameters
    const size_t problem_size = 10;
    const size_t individual_size = 16;

    // Search parameters
    const size_t max_iterations = 30;

    file << "population " << problem_size << " individual_size " << individual_size << " iterations " << max_iterations << '\n';

    // Create population
    std::vector<std::vector<int>> population;
    for (size_t i = 0; i < problem_size; ++i) {
        std::vector<int> problem = melody::create_individual(individual_size);
        population.emplace_back(problem);
    }

    //melody::display(population);
    file << "Initial population \n";
    for (auto const& individual : population) {
        for (auto const& note : individual) {
            file << note << ' ';
        }
        file << '\n';
    }

    file << "Num. individuals: " << population.size() << '\n';

    /*if (midi_file.read("resources/twinkle.midi")){

        midi_file.doTimeAnalysis();

        if (midi_file.getTrackCount() > 1) {

            // Converts multi-track data into a single time sequence.
            // This makes possible to process the event list as a single track list
            midi_file.joinTracks();
        }
    }*/

    // Transform MIDI into binary vector

    /*std::vector<int> v;
    v.push_back(0b10);
    std::cout << std::dec << v.at(0) << std::endl;


    solution_t candidate_solution(midi_file[0]);*/

    std::vector<int> candidate_solution(16);
    std::vector<std::tuple<double,double>> fitness;
    std::tuple<double, double, double, double> evaluation;
    size_t capacity = 100;
    using allocator = std::allocator<std::pair<const ::pareto::point<double, 4>, double>>;
    using Container = pareto::spatial_map<double, 4, double, std::less<double>, allocator>;
    pareto::archive<double, 4, double, Container> pareto_archive(capacity, {pareto::min, pareto::min, pareto::max, pareto::max});
    std::vector<std::vector<int>> solution;
    std::vector<std::tuple<double, int>> crowding_distance;
    double min_valence, min_arousal, max_valence, max_arousal;

    file << "Pareto archive \n";
    for (size_t i = 0; i < problem_size; ++i) {
        evaluation = melody::evaluate(population[i]);
        min_valence = std::get<0>(evaluation);
        min_arousal = std::get<1>(evaluation);
        max_valence = std::get<2>(evaluation);
        max_arousal = std::get<3>(evaluation);
        pareto_archive(min_valence, min_arousal, max_valence, max_arousal) = static_cast<double>(i);
        file << "[" << min_valence << ", " << min_arousal << ", " << max_valence << ", " << max_arousal << "] -> " << i << "\n";
    }

    for (size_t i = 0; i < max_iterations; ++i) {
        std::cout << "Solution #" << i + 1 << std::endl;
        file << "Solution # " << i << "\n";
        for (size_t j = 0; j < problem_size; ++j) {

            // Selection and crossover
            std::uniform_int_distribution<int> d(0, (problem_size-1));
            candidate_solution = melody::crossover(population[d(generator_)], population[d(generator_)]);

            // Mutation
            melody::simple_mutation(candidate_solution, 0.05);

            population.emplace_back(candidate_solution);

            // Evaluation
            evaluation = melody::evaluate(candidate_solution);
            min_valence = std::get<0>(evaluation);
            min_arousal = std::get<1>(evaluation);
            max_valence = std::get<2>(evaluation);
            max_arousal = std::get<3>(evaluation);

            // No dominated ranking
            pareto_archive(min_valence, min_arousal, max_valence, max_arousal) = static_cast<double>(j+problem_size);
        }

        auto it = pareto_archive.begin_front();
        std::cout << "size: " << it->size() << std::endl;

        /*std::cout << "Number of fronts: " << pareto_archive.size_fronts() << std::endl;

        std::cout << "Front Iterators:" << std::endl;
        for (auto it = pareto_archive.begin_front(); it != pareto_archive.end_front(); ++it) {
            std::cout << "Front with " << it->size() << " elements" << std::endl;
            for (const auto &[k, v] : *it) {
                std::cout << k << " -> " << v << std::endl;
            }
        }*/

        if (it->size() > problem_size) {
            for (const auto &[k, v] : pareto_archive) {
                crowding_distance.push_back({pareto_archive.crowding_distance(k),v});
            }

            std::sort(crowding_distance.begin(), crowding_distance.end(), std::greater<std::tuple<double, int>>());

            for (size_t x = 0; x < problem_size; ++x) {
                int position = std::get<1>(crowding_distance[x]);
                solution.push_back(population[position]);
            }
        }
        else if (it->size() < problem_size) {
            // não sestá inserindo individuos que nao fazem parte de nenhuma fronteira
            for (it = pareto_archive.begin_front(); (it != pareto_archive.end_front() || solution.size() == problem_size); ++it) {
                for (const auto &[k, v] : pareto_archive) {
                    solution.push_back(population[v]);
                }

            }
        }
        else {
            for (const auto &[k, v] : pareto_archive) {
                solution.push_back(population[v]);
            }
        }

        population.clear();
        //consertar a solução, que agora só tem um vetor
        for (const std::vector<int> &individual : solution) {
            population.emplace_back(individual);
        }

        file << "Selected population \n";
        for (auto const& individual : population) {
            for (auto const& note : individual) {
                file << note << ' ';
            }
            file << '\n';
        }

        for (auto it = pareto_archive.begin_front(); it != pareto_archive.end_front(); ++it) {
            file << "Front with " << it->size() << " elements" << "\n";
            for (const auto &[k, v] : *it) {
                file << k << " -> " << v << "\n";
            }
        }

        std::ostringstream archive_stream;
        archive_stream << "archive_" << execution << ".svg";
        std::string archive_name = archive_stream.str();

        matplot::hold(false);
        matplot::save(archive_name);

        solution.clear();
    }

    //std::cout << "Solution:" << std::endl;
    //melody::display(population);
    file << "Final solution\n";
    for (auto const& individual : population) {
        for (auto const& note : individual) {
            file << note << ' ';
        }
        file << '\n';
    }

    // If number of individuals in rank > melody.size
        // Crowding distance
        // Sort rank by crowding distance
        // Insert better crowding distance until limit of solution size
    // Else if individuals in rank < melody.size
        // Insert next no dominated set until fill the gap
    // Else
        // Insert rank individuals in population


    file << "Number of fronts: " << pareto_archive.size_fronts() << "\n";

    file << "Front Iterators:" << "\n";
    for (auto it = pareto_archive.begin_front(); it != pareto_archive.end_front(); ++it) {
        file << "Front with " << it->size() << " elements" << "\n";
        for (const auto &[k, v] : *it) {
            file << k << " -> " << v << "\n";
        }
    }

    /*std::cout << "Solution:" << std::endl;
    //errado, a população final ficou bem maior
    for (int i = 0; i < problem_size; ++i) {
        for (int j = 0; j < population[i].size(); ++j) {
            std::cout << population[i][j] << " ";
        }
        std::cout << std::endl;
    }*/

    // Plot a single 2d archive
    //ver se vai funcionar com o novo archive de 4 elementos

    std::ostringstream archive_stream;
    archive_stream << "final_archive_" << execution << ".svg";
    std::string archive_name = archive_stream.str();

    matplot::hold(false);
    pareto::plot_archive(pareto_archive);
    matplot::save(archive_name);
    matplot::show();

    auto it = pareto_archive.begin_front();
    pareto::front<double,2,double> p_front;
    std::vector<std::vector<int>> final_solution;

    for (const auto &[k, v] : *it) {
        std::cout << k << " -> " << v << std::endl;
        p_front(k.get<0>(),k.get<1>()) = v;
        final_solution.emplace_back(population[v]);
    }

    melody::display(final_solution);
    //verificar se está mostrando o archive inteiro ou só a frente e depois salvar
    matplot::hold(false);
    pareto::plot_front(p_front);
    matplot::save("pareto2d.svg");
    matplot::show();

    smf::Options options;
    //options.define("n|note-count=i:10", "How many notes to randomly play");
    //options.define("o|output-file=s",   "Output filename (stdout if none)");
    //options.define("i|instrument=i:0",  "General MIDI instrument number");
    //options.define("x|hex=b",           "Hex byte-code output");

    //random_device rd;
    //mt19937 mt(rd());
    //uniform_int_distribution<int> starttime(0, 100);
    //uniform_int_distribution<int> duration(1, 8);
    //uniform_int_distribution<int> pitch(36, 84);
    //uniform_int_distribution<int> velocity(40, 100);

    smf::MidiFile midifile;
    int track   = 0;
    int channel = 0;
    int instr   = 24;
    midifile.addTimbre(track, 0, channel, instr);

    int tpq     = midifile.getTPQ();
    //int count   = options.getInteger("note-count");
    int starttick = 0;
    int endtick   = starttick + int(5 / 4.0 * tpq);

    for (int i=0; i<final_solution.size(); i++) {
        for (int j = 0; j < 16; ++j) {

            int key       = final_solution[i][j];
            midifile.addNoteOn (track, starttick, channel, key, 50);
            midifile.addNoteOff(track, endtick,   channel, key);
            starttick += endtick;
            endtick   = int(5 / 4.0 * tpq);
        }
    }
    midifile.sortTracks();  // Need to sort tracks since added events are
                           // appended to track in random tick order.
    std::string filename = "exp-01.midi";
    if (filename.empty()) {
        if (options.getBoolean("hex")) midifile.writeHex(std::cout);
        else std::cout << midifile;
    } else
        midifile.write(filename);

    file.close();
}

int main() {

    for (int i = 0; i < 1; ++i) {
        nsga_ii(i);
    }

    return 0;
}