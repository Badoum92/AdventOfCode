#include "common.hh"

struct Food
{
    Food(std::set<std::string> i, std::set<std::string> a)
        : ingredients(std::move(i))
        , allergens(std::move(a))
    {}

    std::set<std::string> ingredients;
    std::set<std::string> allergens;
};

static std::vector<Food> parse_input(const std::vector<std::string>& lines,
                                     std::vector<std::string>& ingredient_list)
{
    std::vector<Food> ret;
    for (const auto& line : lines)
    {
        std::set<std::string> ingredients;
        std::set<std::string> allergens;

        auto tokens = split(line, " ");
        unsigned i = 0;
        for (; tokens[i] != "(contains"; ++i)
        {
            ingredients.insert(tokens[i]);
        }
        i++;
        for (; i < tokens.size(); ++i)
        {
            tokens[i].pop_back();
            allergens.insert(tokens[i]);
        }

        ret.emplace_back(ingredients, allergens);

        ingredient_list.insert(end(ingredient_list), begin(ingredients),
                               end(ingredients));
    }
    return ret;
}

static std::unordered_map<std::string, std::set<std::string>>
map_food(const std::vector<Food>& food_list)
{
    std::unordered_map<std::string, std::set<std::string>> ret;

    for (const auto& food : food_list)
    {
        for (const auto& i : food.allergens)
        {
            ret[i].insert(begin(food.ingredients), end(food.ingredients));
        }
    }

    for (const auto& food : food_list)
    {
        for (const auto& i : food.allergens)
        {
            std::set<std::string> new_set;
            std::set_intersection(
                begin(ret[i]), end(ret[i]), begin(food.ingredients),
                end(food.ingredients), std::inserter(new_set, begin(new_set)));
            std::swap(new_set, ret[i]);
        }
    }

    return ret;
}

static unsigned step1(const char* input_file)
{
    std::ifstream input(input_file);
    auto lines = input_to_lines(input);
    std::vector<std::string> ingredient_list;
    auto food_list = parse_input(lines, ingredient_list);

    auto map = map_food(food_list);
    std::set<std::string> contains_allergens;
    for (const auto& [key, val] : map)
    {
        contains_allergens.insert(begin(val), end(val));
    }

    ingredient_list.erase(std::remove_if(begin(ingredient_list),
                                         end(ingredient_list),
                                         [&](const std::string& s) {
                                             return contains_allergens.count(s);
                                         }),
                          end(ingredient_list));

    return ingredient_list.size();
}

static std::vector<std::pair<std::string, std::string>>
find_dangerous(std::unordered_map<std::string, std::set<std::string>>& map)
{
    std::vector<std::pair<std::string, std::string>> dangerous;
    for (auto it = begin(map); it != end(map);)
    {
        auto& [key, val] = *it;

        if (val.size() == 1)
        {
            std::string ingredient = *begin(val);
            dangerous.emplace_back(ingredient, key);
            for (auto& [k, v] : map)
            {
                v.erase(ingredient);
            }
            it = begin(map);
        }
        else
        {
            ++it;
        }
    }
    return dangerous;
}

static std::string step2(const char* input_file)
{
    std::ifstream input(input_file);
    auto lines = input_to_lines(input);
    std::vector<std::string> ingredient_list;
    auto food_list = parse_input(lines, ingredient_list);
    auto map = map_food(food_list);
    auto dangerous = find_dangerous(map);

    std::sort(
        begin(dangerous), end(dangerous),
        [&](const auto& a, const auto& b) { return a.second < b.second; });

    std::string answer;
    for (const auto& [i, a] : dangerous)
    {
        answer += i + ",";
    }
    answer.pop_back();
    return answer;
}

void Day21::run()
{
    std::cout << "Day21:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;
    std::cout << "    step1: " << step1(input_file) << "\n";
    std::cout << "    step2: " << step2(input_file) << "\n\n";
}

bool Day21::sanity_check()
{
    unsigned step1_expected = 5;
    std::string step2_expected = "mxmxvkd,sqjhc,fvjkl";
    bool ret = true;

    unsigned step1_got = step1(provided_file);
    if (step1_got != step1_expected)
    {
        fprintf(stderr,
                "\n    step1 sanity check failed: expected %u, got %u\n",
                step1_expected, step1_got);
        ret = false;
    }

    std::string step2_got = step2(provided_file);
    if (step2_got != step2_expected)
    {
        fprintf(stderr,
                "\n    step2 sanity check failed: expected %s, got %s\n",
                step2_expected.c_str(), step2_got.c_str());
        ret = false;
    }

    if (ret)
        std::cout << " OK\n";
    return ret;
}
