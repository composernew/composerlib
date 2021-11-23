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

using allocator = std::allocator<std::pair<const ::pareto::point<double, 4>, double>>;
using Container = pareto::spatial_map<double, 4, double, std::less<double>, allocator>;

std::tuple<std::tuple<double,double>, std::tuple<double,double>, std::tuple<double,double>, std::tuple<double,double>,
    int, int, int, int, size_t,
           pareto::archive<double,4,double,Container>>
    nsga_ii(int execution, int problem_size, int individual_size, int max_iterations, double mutation_strength) {

    static std::default_random_engine generator_;
    generator_ = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

    std::ostringstream stream;
    stream << "experiment_" << execution << ".txt";
    std::string experiment = stream.str();

    std::ofstream file(experiment);
    file << "Execution " << execution << " ";

    // Create population
    // Gera uma população inicialcom N indivíduos a partir deuma melodia de referência
    std::vector<std::vector<int>> population;
    for (size_t i = 0; i < problem_size; ++i) {
        std::vector<int> problem = {60, 62, 64, 65, 67, 69, 71, 72, 60, 62, 64, 65, 67, 69, 71, 72};//melody::create_individual(individual_size);
        population.emplace_back(problem);
    }

    file << "population: " << problem_size << "\nindividual_size: " << individual_size
         << "\niterations " << max_iterations << "\nmutation strength: " << mutation_strength << '\n';
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

    std::vector<int> candidate_solution(individual_size);
    std::vector<std::tuple<double,double>> fitness;
    std::tuple<double, double, double, double> evaluation;
    size_t capacity = 1000;

    pareto::archive<double, 4, double, Container> pareto_archive(capacity, {pareto::min, pareto::min, pareto::max, pareto::max});
    std::vector<std::vector<int>> solution;
    std::vector<std::tuple<double, int>> crowding_distance;
    double min_valence, min_arousal, max_valence, max_arousal;

    // Best solution
    std::tuple<double,double> best_min_min = {2.,2.};
    std::tuple<double,double> best_min_max = {2.,-2.};
    std::tuple<double,double> best_max_min = {-2.,2.};
    std::tuple<double,double> best_max_max = {-2.,-2.};

    int min_min_gen = -1;
    int max_max_gen = -1;
    int min_max_gen = -1;
    int max_min_gen = -1;

    std::array<std::vector<int>,4> best_melodies;

    file << "Init Pareto archive \n";
    // Avalia população inicial combase nas características musicais
    for (size_t i = 0; i < problem_size; ++i) {
        evaluation = melody::evaluate(population[i], individual_size);
        min_valence = std::get<0>(evaluation);
        min_arousal = std::get<1>(evaluation);
        max_valence = std::get<2>(evaluation);
        max_arousal = std::get<3>(evaluation);
        pareto_archive(min_valence, min_arousal, max_valence, max_arousal) = static_cast<double>(i);
    }

    std::ostringstream archive_stream;
    archive_stream << "initial_archive_" << execution << ".svg";
    std::string archive_name = archive_stream.str();

    /*matplot::hold(false);
    pareto::plot_archive(pareto_archive);
    matplot::save(archive_name);
    matplot::show();*/

    auto it_i = pareto_archive.begin_front();
    pareto::front<double,4,double> p_front_i;
    std::vector<std::vector<int>> initial_solution;

    for (const auto &[k, v] : *it_i) {
        std::cout << k << " -> " << v << std::endl;
        p_front_i(k.get<0>(),k.get<1>(), k.get<2>(), k.get<3>()) = v;
        initial_solution.emplace_back(population[v]);
    }

    /*matplot::hold(false);
    pareto::plot_front(p_front_i);
    matplot::save("pareto_init.svg");
    matplot::show();*/

    std::cout << "Exact hypervolume: " << p_front_i.hypervolume(p_front_i.nadir()) << std::endl;

    file << "Fitness inicial: " << "[" << min_valence << ", " << min_arousal << ", " << max_valence << ", " << max_arousal << "] \n";

    std::vector<std::vector<int>> final_population;

    // Aplica seleção dos pais, crossover e mutação
    for (size_t i = 0; i < max_iterations; ++i) {
        //std::cout << "Solution #" << i + 1 << std::endl;
        //file << "Solution # " << i << "\n";
        for (size_t j = 0; j < problem_size; ++j) {

            // Selection and crossover
            std::uniform_int_distribution<int> d(0, (problem_size-1));
            candidate_solution = melody::crossover(population[d(generator_)], population[d(generator_)]);

            // Mutation
            melody::simple_mutation(candidate_solution, mutation_strength);

            population.emplace_back(candidate_solution);

            // Evaluation
            evaluation = melody::evaluate(candidate_solution, individual_size);
            min_valence = std::get<0>(evaluation);
            min_arousal = std::get<1>(evaluation);
            max_valence = std::get<2>(evaluation);
            max_arousal = std::get<3>(evaluation);

            // No dominated ranking
            pareto_archive(min_valence, min_arousal, max_valence, max_arousal) = static_cast<double>(j+problem_size);

            if((std::get<0>(best_min_min) >= min_valence) && (std::get<1>(best_min_min) >= min_arousal)
                && (min_valence != 0) && (min_arousal != 0)) {
                best_min_min = {min_valence, min_arousal};
                min_min_gen = i;
                best_melodies[0] = candidate_solution;
            }

            if((std::get<0>(best_max_max) <= max_valence) && (std::get<1>(best_max_max) <= max_arousal)
                && (max_valence != 0) && (max_arousal != 0)) {
                best_max_max = {max_valence, max_arousal};
                max_max_gen = i;
                best_melodies[1] = candidate_solution;
            }

            if((std::get<0>(best_min_max) >= min_valence) && (std::get<1>(best_min_max) <= max_arousal)
                && (min_valence != 0) && (max_arousal != 0)) {
                best_min_max = {min_valence, max_arousal};
                min_max_gen = i;
                best_melodies[2] = candidate_solution;
            }

            if((std::get<0>(best_max_min) <= max_valence) && (std::get<1>(best_max_min) >= min_arousal)
                && (max_valence != 0) && (min_arousal != 0)) {
                best_max_min = {max_valence, min_arousal};
                max_min_gen = i;
                best_melodies[3] = candidate_solution;
            }
        }

        auto it = pareto_archive.begin_front();
        //std::cout << "size: " << it->size() << std::endl;

        /*std::cout << "Number of fronts: " << pareto_archive.size_fronts() << std::endl;

        std::cout << "Front Iterators:" << std::endl;
        for (auto it = pareto_archive.begin_front(); it != pareto_archive.end_front(); ++it) {
            std::cout << "Front with " << it->size() << " elements" << std::endl;
            for (const auto &[k, v] : *it) {
                std::cout << k << " -> " << v << std::endl;
            }
        }*/

        //std::cout << "i: " << i << std::endl;

        while ((it->size() < (problem_size - solution.size()))) {
            for (const auto &[k, v] : *it) {
                solution.push_back(population[v]);
            }
            ++it;

            if (it == pareto_archive.end_front())
                break;
        }

        if (it != pareto_archive.end_front()) {
            if (it->size() > (problem_size - solution.size())) {
                for (const auto &[k, v] : *it) {
                    crowding_distance.push_back({pareto_archive.crowding_distance(k),v});
                }

                std::sort(crowding_distance.begin(), crowding_distance.end(), std::greater<std::tuple<double, int>>());

                for (size_t x = 0; solution.size() < problem_size; ++x) {
                    int position = std::get<1>(crowding_distance[x]);
                    solution.push_back(population[position]);
                }
            }
        }

        if (i == (max_iterations-1)) {
            final_population = population;
        }

        // gambiarra para não inserir valores que já estão na solução
        for (auto iterator = pareto_archive.begin_front(); iterator != pareto_archive.end_front(); ++iterator) {
            for (const auto &[k, v] : *iterator) {
                population[v] = {};
            }
        }

        // gambiarra para completar a população quando a quantidade de pontos no archive não for suficiente para encher
        int index = 0;
        while (solution.size() < problem_size) {
            if(!population[index].empty()) {
                solution.push_back(population[index]);
            }
            ++index;
        }

        /*if (it->size() > problem_size) {
            std::cout << "it->size() > problem_size" << std::endl;

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
            std::cout << "it->size() < problem_size" << std::endl;
            // não está inserindo individuos que nao fazem parte de nenhuma fronteira
            for (it = pareto_archive.begin_front(); (it != pareto_archive.end_front() || solution.size() == problem_size); ++it) {
                for (const auto &[k, v] : pareto_archive) {
                    solution.push_back(population[v]);
                }

            }
        }
        else {
            std::cout << "else" << std::endl;
            for (const auto &[k, v] : pareto_archive) {
                solution.push_back(population[v]);
            }
        }*/

        population.clear();
        //consertar a solução, que agora só tem um vetor
        for (const std::vector<int> &individual : solution) {
            population.emplace_back(individual);
        }

        /*file << "Selected population \n";
        for (auto const& individual : population) {
            for (auto const& note : individual) {
                file << note << ' ';
            }
            file << '\n';
        }*/

        /*for (auto it = pareto_archive.begin_front(); it != pareto_archive.end_front(); ++it) {
            file << "Front with " << it->size() << " elements" << "\n";
            for (const auto &[k, v] : *it) {
                file << k << " -> " << v << "\n";
            }
        }*/

        /*std::ostringstream archive_stream;
        archive_stream << "archive_" << execution << ".svg";
        std::string archive_name = archive_stream.str();

        matplot::hold(false);
        matplot::save(archive_name);*/

        solution.clear();
    }

    file << "Best melodies - execution " << execution << '\n';

    for (const auto &result : best_melodies) {
        for (int note : result) {
            file << note << ' ';
        }
        file << '\n';
    }

    //std::cout << "Solution:" << std::endl;
    //melody::display(population);
    /*file << "Final solution\n";
    for (auto const& individual : population) {
        for (auto const& note : individual) {
            file << note << ' ';
        }
        file << '\n';
    }*/

    // If number of individuals in rank > number of individuals
        // Crowding distance
        // Sort rank by crowding distance
        // Insert better crowding distance until limit of solution size
    // Else if individuals in rank < melody.size
        // Insert next no dominated set until fill the gap
    // Else
        // Insert rank individuals in population

    /*double min_valence_media = 0;
    double max_valence_media = 0;
    double min_arousal_media = 0;
    double max_arousal_media = 0;*/

    /*double best_min_valence = 0;
    double best_max_valence = 0;
    double best_min_arousal = 0;
    double best_max_arousal = 0;*/

    //file << "Number of fronts: " << pareto_archive.size_fronts() << "\n";

    //file << "Front Iterators:" << "\n";
    auto it = pareto_archive.begin_front();
    //std::cout << "Front with " << it->size() << " elements" << "\n";
    double size = static_cast<double>(it->size());
    //for (auto it = pareto_archive.begin_front(); it != pareto_archive.end_front(); ++it) {
    file << "Front with " << it->size() << " elements" << "\n";
    for (const auto &[k, v] : *it) {
        //std::cout << "Front with " << it->size() << " elements" << "\n";
        //double size = static_cast<double>(it->size());
        //for (const auto &[k, v] : *it) {
            file << k << " -> " << v << "\n";
            //std::cout << k << " -> " << v << "\n";

            /*min_valence_media += k.get<0>();
            max_valence_media += k.get<2>();
            min_arousal_media += k.get<1>();
            max_arousal_media += k.get<3>();*/

            /*best_min_valence = std::min(k.get<0>(),best_min_valence);
            best_max_valence = std::max(k.get<2>(),best_max_valence);
            best_min_arousal = std::min(k.get<1>(),best_min_arousal);
            best_max_arousal = std::max(k.get<3>(),best_max_arousal);*/

            for (auto const& note : final_population[v]) {
                file << note << ' ';
            }
            file << '\n';
        //}

        /*min_valence_media = min_valence_media/size;
        max_valence_media = max_valence_media/size;
        min_arousal_media = min_arousal_media/size;
        max_arousal_media = max_arousal_media/size;*/
    }

    /*file << "Min Valence media: " << min_valence_media << '\n';
    file << "Max Valence media: " << max_valence_media << '\n';
    file << "Min Arousal media: " << min_arousal_media << '\n';
    file << "Max Arousal media: " << max_arousal_media << '\n';*/

    /*file << "Best Min Valence: " << best_min_valence << '\n';
    file << "Best Max Valence: " << best_max_valence << '\n';
    file << "Best Min Arousal: " << best_min_arousal << '\n';
    file << "Best Max Arousal: " << best_max_arousal << '\n';*/

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

    /*std::ostringstream final_archive_stream;
    final_archive_stream << "final_archive_" << execution << ".svg";
    std::string final_archive_name = final_archive_stream.str();*/

    /*//matplot::hold(false);
    pareto::plot_archive(pareto_archive);
    matplot::save(archive_name);
    matplot::show();//*/

    pareto::front<double,4,double> p_front;
    //std::vector<std::vector<int>> final_solution;

    size_t final_num_individuals = it->size();


    for (const auto &[k, v] : *it) {
        std::cout << k << " -> " << v << std::endl;
        p_front(k.get<0>(),k.get<1>(), k.get<2>(), k.get<3>()) = v;
        //final_solution.emplace_back(population[v]);
    }

    //melody::display(final_solution);


    //verificar se está mostrando o archive inteiro ou só a frente e depois salvar
    /*matplot::hold(false);
    pareto::plot_front(p_front);
    matplot::save("pareto2d.svg");
    matplot::show();*/

    /*smf::Options options;
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
        midifile.write(filename);*/

    file.close();

    return {best_min_min, best_min_max, best_max_min, best_max_max, min_min_gen, max_max_gen, max_min_gen, min_max_gen,
            final_num_individuals, pareto_archive};
}

int main() {

    std::ostringstream stream;
    stream << "experiment_result.txt";
    std::string experiment = stream.str();
    std::ofstream file(experiment);

    std::ofstream file_archives("archives.txt");

    // Best solution
    std::tuple<double,double> best_min_min = {2.,2.};
    std::tuple<double,double> best_min_max = {2.,-2.};
    std::tuple<double,double> best_max_min = {-2.,2.};
    std::tuple<double,double> best_max_max = {-2.,-2.};

    int min_min_gen = -1;
    int max_max_gen = -1;
    int min_max_gen = -1;
    int max_min_gen = -1;

    int min_min_exec = -1;
    int max_max_exec = -1;
    int min_max_exec = -1;
    int max_min_exec = -1;

    int num_executions = 100;

    int mean_num_individuals = 0;

    for (int i = 0; i < num_executions; ++i) {

        // Problem parameters
        const size_t problem_size = 1000;
        const size_t individual_size = 16;

        // Search parameters
        const size_t max_iterations = 100;
        double mutation_strength = 0.05;

        std::tuple<std::tuple<double,double>, std::tuple<double,double>, std::tuple<double,double>, std::tuple<double,double>,
                   int, int, int, int, size_t,pareto::archive<double,4,double,Container>> result;

        result = nsga_ii(i, problem_size, individual_size, max_iterations, mutation_strength);
        double min_min_valence = std::get<0>(std::get<0>(result));
        double min_min_arousal = std::get<1>(std::get<0>(result));
        double max_max_valence = std::get<0>(std::get<3>(result));
        double max_max_arousal = std::get<1>(std::get<3>(result));
        double max_min_valence = std::get<0>(std::get<2>(result));
        double max_min_arousal = std::get<1>(std::get<2>(result));
        double min_max_valence = std::get<0>(std::get<1>(result));
        double min_max_arousal = std::get<1>(std::get<1>(result));

        if((std::get<0>(best_min_min) >= min_min_valence) && (std::get<1>(best_min_min) >= min_min_arousal)) {
            best_min_min = {min_min_valence, min_min_arousal};
            min_min_gen = std::get<4>(result);
            min_min_exec = i;
        }

        if((std::get<0>(best_max_max) <= max_max_valence) && (std::get<1>(best_max_max) <= max_max_arousal)) {
            best_max_max = {max_max_valence, max_max_arousal};
            max_max_gen = std::get<5>(result);
            max_max_exec = i;
        }

        if((std::get<0>(best_min_max) >= min_max_valence) && (std::get<1>(best_min_max) <= min_max_arousal)) {
            best_min_max = {min_max_valence, min_max_arousal};
            min_max_gen = std::get<7>(result);
            min_max_exec = i;
        }

        if((std::get<0>(best_max_min) <= max_min_valence) && (std::get<1>(best_max_min) >= max_min_arousal)) {
            best_max_min = {max_min_valence, max_min_arousal};
            max_min_gen = std::get<6>(result);
            max_min_exec = i;
        }

        mean_num_individuals += std::get<8>(result);

        pareto::archive<double,4,double,Container> pareto_archive = std::get<9>(result);

        auto it = pareto_archive.begin_front();

        for (const auto &[k, v] : *it) {
            file_archives << k.get<0>() << ',' << k.get<1>() << ',' << k.get<2>() << ',' << k.get<3>() << '\n';
        }
    }

    file << "Best result:\n";

    file << "min_min\n";
    file << "Gen: " << min_min_gen << " Exec.: " << min_min_exec << '\n';
    file << '[' << std::get<0>(best_min_min) << ',' << std::get<1>(best_min_min) << "]\n";

    file << "max_max\n";
    file << "Gen: " << max_max_gen << " Exec.: " << max_max_exec << '\n';
    file << '[' << std::get<0>(best_max_max) << ',' << std::get<1>(best_max_max) << "]\n";

    file << "min_max\n";
    file << "Gen: " << min_max_gen << " Exec.: " << min_max_exec << '\n';
    file << '[' << std::get<0>(best_min_max) << ',' << std::get<1>(best_min_max) << "]\n";

    file << "max_min\n";
    file << "Gen: " << max_min_gen << " Exec.: " << max_min_exec << '\n';
    file << '[' << std::get<0>(best_max_min) << ',' << std::get<1>(best_max_min) << "]\n";

    mean_num_individuals = mean_num_individuals/num_executions;

    file << "Mean num. individuals: " << mean_num_individuals << '\n';

    file.close();
    file_archives.close();

    return 0;
}