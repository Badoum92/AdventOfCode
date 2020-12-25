// This is terrible, please end me

#include "common.hh"

#include <bitset>

enum Permutation
{
    ROT_0,
    ROT_90,
    ROT_180,
    ROT_270,
    H_FLIP_ROT_0,
    H_FLIP_ROT_90,
    H_FLIP_ROT_180,
    H_FLIP_ROT_270,
    V_FLIP_ROT_0,
    V_FLIP_ROT_90,
    V_FLIP_ROT_180,
    V_FLIP_ROT_270,
    HV_FLIP_ROT_0,
    HV_FLIP_ROT_90,
    HV_FLIP_ROT_180,
    HV_FLIP_ROT_270
};

static std::array<Permutation, 16> permutations = {
    ROT_0,         ROT_90,         ROT_180,         ROT_270,
    H_FLIP_ROT_0,  H_FLIP_ROT_90,  H_FLIP_ROT_180,  H_FLIP_ROT_270,
    V_FLIP_ROT_0,  V_FLIP_ROT_90,  V_FLIP_ROT_180,  V_FLIP_ROT_270,
    HV_FLIP_ROT_0, HV_FLIP_ROT_90, HV_FLIP_ROT_180, HV_FLIP_ROT_270};

enum Direction
{
    UP,
    RIGHT,
    DOWN,
    LEFT
};

static std::array<Direction, 4> directions = {UP, RIGHT, DOWN, LEFT};

struct Tile;
using Edges = std::array<std::bitset<10>, 4>;
using TiledImage = std::vector<std::vector<Tile*>>;

static std::bitset<10> flip_bits(const std::bitset<10>& bits)
{
    std::bitset<10> ret;
    for (unsigned i = 0; i < 10; ++i)
    {
        ret[i] = bits[10 - i - 1];
    }
    return ret;
}

static std::vector<std::string> rotate_90(const std::vector<std::string>& v)
{
    std::vector<std::string> new_v(v[0].size(), std::string(v.size(), ' '));
    for (unsigned i = 0; i < new_v.size(); ++i)
    {
        for (unsigned j = 0; j < new_v[i].size(); ++j)
        {
            new_v[i][j] = v[v.size() - j - 1][i];
        }
    }
    return new_v;
}

static std::vector<std::string> h_flip(const std::vector<std::string>& v)
{
    std::vector<std::string> new_v = v;
    for (auto& line : new_v)
    {
        std::reverse(begin(line), end(line));
    }
    return new_v;
}

static std::vector<std::string> v_flip(const std::vector<std::string>& v)
{
    std::vector<std::string> new_v = v;
    std::reverse(begin(new_v), end(new_v));
    return new_v;
}

struct Match
{
    Match(Tile* t, Direction d, Permutation p)
        : tile(t)
        , dir(d)
        , perm(p)
    {}

    Tile* tile;
    Direction dir;
    Permutation perm;
};

struct Tile
{
    Tile(uint64_t n)
        : number(n)
    {}

    uint64_t number;
    std::vector<std::string> tile;
    std::array<Edges, 16> edges;

    void gen_permutations()
    {
        gen_no_flip();
        gen_h_flip();
        gen_v_flip();
        gen_hv_flip();
    }
    void gen_rot(Edges& cur, const Edges& ref)
    {
        cur[UP] = flip_bits(ref[LEFT]);
        cur[RIGHT] = ref[UP];
        cur[DOWN] = flip_bits(ref[RIGHT]);
        cur[LEFT] = ref[DOWN];
    }
    void gen_no_flip()
    {
        for (unsigned i = 0; i < 10; ++i)
        {
            unsigned rev_i = 10 - i - 1;
            edges[ROT_0][UP][rev_i] = (tile[0][i] == '#');
            edges[ROT_0][RIGHT][rev_i] = (tile[i][9] == '#');
            edges[ROT_0][DOWN][rev_i] = (tile[9][i] == '#');
            edges[ROT_0][LEFT][rev_i] = (tile[i][0] == '#');
        }
        gen_rot(edges[ROT_90], edges[ROT_0]);
        gen_rot(edges[ROT_180], edges[ROT_90]);
        gen_rot(edges[ROT_270], edges[ROT_180]);
    }
    void gen_h_flip()
    {
        for (unsigned i = 0; i < 10; ++i)
        {
            unsigned rev_i = 10 - i - 1;
            edges[H_FLIP_ROT_0][UP][rev_i] = (tile[0][rev_i] == '#');
            edges[H_FLIP_ROT_0][RIGHT][rev_i] = (tile[i][0] == '#');
            edges[H_FLIP_ROT_0][DOWN][rev_i] = (tile[9][rev_i] == '#');
            edges[H_FLIP_ROT_0][LEFT][rev_i] = (tile[i][9] == '#');
        }
        gen_rot(edges[H_FLIP_ROT_90], edges[H_FLIP_ROT_0]);
        gen_rot(edges[H_FLIP_ROT_180], edges[H_FLIP_ROT_90]);
        gen_rot(edges[H_FLIP_ROT_270], edges[H_FLIP_ROT_180]);
    }
    void gen_v_flip()
    {
        for (unsigned i = 0; i < 10; ++i)
        {
            unsigned rev_i = 10 - i - 1;
            edges[V_FLIP_ROT_0][UP][rev_i] = (tile[9][i] == '#');
            edges[V_FLIP_ROT_0][RIGHT][rev_i] = (tile[rev_i][9] == '#');
            edges[V_FLIP_ROT_0][DOWN][rev_i] = (tile[0][i] == '#');
            edges[V_FLIP_ROT_0][LEFT][rev_i] = (tile[rev_i][0] == '#');
        }
        gen_rot(edges[V_FLIP_ROT_90], edges[V_FLIP_ROT_0]);
        gen_rot(edges[V_FLIP_ROT_180], edges[V_FLIP_ROT_90]);
        gen_rot(edges[V_FLIP_ROT_270], edges[V_FLIP_ROT_180]);
    }
    void gen_hv_flip()
    {
        for (unsigned i = 0; i < 10; ++i)
        {
            unsigned rev_i = 10 - i - 1;
            edges[HV_FLIP_ROT_0][UP][rev_i] = (tile[9][rev_i] == '#');
            edges[HV_FLIP_ROT_0][RIGHT][rev_i] = (tile[rev_i][0] == '#');
            edges[HV_FLIP_ROT_0][DOWN][rev_i] = (tile[0][rev_i] == '#');
            edges[HV_FLIP_ROT_0][LEFT][rev_i] = (tile[rev_i][9] == '#');
        }
        gen_rot(edges[HV_FLIP_ROT_90], edges[HV_FLIP_ROT_0]);
        gen_rot(edges[HV_FLIP_ROT_180], edges[HV_FLIP_ROT_90]);
        gen_rot(edges[HV_FLIP_ROT_270], edges[HV_FLIP_ROT_180]);
    }

    std::optional<Match> match(Tile& other) const
    {
        for (const auto& dir : directions)
        {
            for (const auto& perm : permutations)
            {
                if (edges[ROT_0][dir] == other.edges[perm][(dir + 2) % 4])
                {
                    return Match(&other, dir, perm);
                }
            }
        }
        return std::nullopt;
    }

    void rotate(Permutation angle)
    {
        if (angle == ROT_90 || angle == H_FLIP_ROT_90 || angle == V_FLIP_ROT_90
            || angle == HV_FLIP_ROT_90)
        {
            tile = rotate_90(tile);
        }
        else if (angle == ROT_180 || angle == H_FLIP_ROT_180
                 || angle == V_FLIP_ROT_180 || angle == HV_FLIP_ROT_180)
        {
            tile = rotate_90(tile);
            tile = rotate_90(tile);
        }
        else if (angle == ROT_270 || angle == H_FLIP_ROT_270
                 || angle == V_FLIP_ROT_270 || angle == HV_FLIP_ROT_270)
        {
            tile = rotate_90(tile);
            tile = rotate_90(tile);
            tile = rotate_90(tile);
        }
    }

    void flip(Permutation side)
    {
        if (side == H_FLIP_ROT_0 || side == H_FLIP_ROT_90
            || side == H_FLIP_ROT_180 || side == H_FLIP_ROT_270)
        {
            tile = h_flip(tile);
        }
        else if (side == V_FLIP_ROT_0 || side == V_FLIP_ROT_90
                 || side == V_FLIP_ROT_180 || side == V_FLIP_ROT_270)
        {
            tile = v_flip(tile);
        }
        else if (side == HV_FLIP_ROT_0 || side == HV_FLIP_ROT_90
                 || side == HV_FLIP_ROT_180 || side == HV_FLIP_ROT_270)
        {
            tile = h_flip(tile);
            tile = v_flip(tile);
        }
    }

    void transform(Permutation permutation)
    {
        flip(permutation);
        rotate(permutation);
        gen_permutations();
    }

    void remove_border()
    {
        tile.erase(begin(tile));
        tile.pop_back();

        for (auto& line : tile)
        {
            line.erase(begin(line));
            line.pop_back();
        }
    }
};

static std::vector<Tile> parse_input(std::vector<std::string>& lines)
{
    std::vector<Tile> ret;
    uint64_t n = 0;
    for (auto& l : lines)
    {
        if (l[0] == 'T')
        {
            sscanf(l.c_str(), "Tile %zu:", &n);
            ret.emplace_back(n);
        }
        else if (l == "")
        {
            continue;
        }
        else
        {
            ret.back().tile.push_back(std::move(l));
        }
    }

    for (auto& tile : ret)
    {
        tile.gen_permutations();
    }

    return ret;
}

static std::unordered_map<Tile*, std::vector<Match>>
find_matches(std::vector<Tile>& tiles)
{
    std::unordered_map<Tile*, std::vector<Match>> ret;
    for (auto& t1 : tiles)
    {
        for (auto& t2 : tiles)
        {
            if (t1.number == t2.number)
                continue;
            auto match = t1.match(t2);
            if (match)
            {
                ret[&t1].push_back(*match);
            }
        }
    }
    return ret;
}

static uint64_t step1(const char* input_file)
{
    std::ifstream input(input_file);
    auto lines = input_to_lines(input);
    auto tiles = parse_input(lines);

    auto matches = find_matches(tiles);
    uint64_t res = 1;
    for (const auto& [key, val] : matches)
    {
        if (val.size() == 2)
            res *= key->number;
    }
    return res;
}

static TiledImage create_tiled_image(std::vector<Tile>& tiles)
{
    unsigned n = std::sqrt(tiles.size());
    TiledImage image;
    image.resize(n);
    for (auto& line : image)
    {
        line.resize(n, nullptr);
    }
    auto matches = find_matches(tiles);
    for (const auto& [key, val] : matches)
    {
        if (val.size() == 2)
        {
            if (std::all_of(begin(val), end(val), [&](const Match& m) {
                    return m.dir == RIGHT || m.dir == DOWN;
                }))
            {
                image[0][0] = key;
                break;
            }
        }
    }

    for (unsigned y = 0; y < image.size(); ++y)
    {
        for (unsigned x = 0; x < image[y].size(); ++x)
        {
            Tile* t = image[y][x];
            for (auto& m : matches[t])
            {
                m = *t->match(*m.tile);
                int dir_x = 0;
                int dir_y = 0;
                if (m.dir == UP)
                    dir_y = -1;
                else if (m.dir == RIGHT)
                    dir_x = 1;
                else if (m.dir == DOWN)
                    dir_y = 1;
                else if (m.dir == LEFT)
                    dir_x = -1;

                if (image[y + dir_y][x + dir_x])
                    continue;

                image[y + dir_y][x + dir_x] = m.tile;
                m.tile->transform(m.perm);
            }
        }
    }

    return image;
}

static std::vector<std::string> create_image(const TiledImage& tiled_image)
{
    std::vector<std::string> image;
    image.resize(tiled_image.size() * 8);
    for (unsigned y = 0; y < tiled_image.size(); ++y)
    {
        for (unsigned x = 0; x < tiled_image[y].size(); ++x)
        {
            Tile* tile = tiled_image[y][x];
            tile->remove_border();
            for (unsigned yt = 0; yt < tile->tile.size(); ++yt)
            {
                image[y * 8 + yt].resize(tiled_image[y].size() * 8);
                for (unsigned xt = 0; xt < tile->tile[yt].size(); ++xt)
                {
                    image[y * 8 + yt][x * 8 + xt] = tile->tile[yt][xt];
                }
            }
        }
    }
    return image;
}

static std::array<std::vector<std::string>, 16> gen_monsters()
{
    std::array<std::vector<std::string>, 16> ret;
    ret[0].push_back("                  # ");
    ret[0].push_back("#    ##    ##    ###");
    ret[0].push_back(" #  #  #  #  #  #   ");
    ret[1] = h_flip(ret[0]);
    ret[2] = v_flip(ret[0]);
    ret[3] = v_flip(h_flip(ret[0]));

    for (unsigned i = 0; i < 4; ++i)
    {
        ret[i * 3 + 4] = rotate_90(ret[i]);
        ret[i * 3 + 5] = rotate_90(rotate_90(ret[i]));
        ret[i * 3 + 6] = rotate_90(rotate_90(rotate_90(ret[i])));
    }

    return ret;
}

static bool coord_is_monster(std::vector<std::string>& image,
                             const std::vector<std::string>& monster,
                             unsigned x, unsigned y)
{
    for (unsigned ym = 0; ym < monster.size(); ++ym)
    {
        for (unsigned xm = 0; xm < monster[ym].size(); ++xm)
        {
            if (monster[ym][xm] == '#' && image[y + ym][x + xm] != '#')
                return false;
        }
    }
    return true;
}

static uint64_t find_monster(std::vector<std::string>& image,
                             const std::vector<std::string>& monster)
{
    uint64_t count = 0;
    for (unsigned y = 0; y < image.size() - monster.size(); ++y)
    {
        for (unsigned x = 0; x < image[0].size() - monster[0].size(); ++x)
        {
            if (coord_is_monster(image, monster, x, y))
                ++count;
        }
    }
    return count;
}

static uint64_t step2(const char* input_file)
{
    std::ifstream input(input_file);
    auto lines = input_to_lines(input);
    auto tiles = parse_input(lines);
    auto tiled_image = create_tiled_image(tiles);
    auto image = create_image(tiled_image);
    auto monsters = gen_monsters();

    uint64_t pixels = 0;
    for (const auto& line : image)
    {
        for (const auto& c : line)
        {
            if (c == '#')
                ++pixels;
        }
    }

    for (unsigned i = 0; i < monsters.size(); ++i)
    {
        uint64_t monster_count = find_monster(image, monsters[i]);
        if (monster_count != 0)
            return pixels - 15 * monster_count;
    }

    return 0;
}

void Day20::run()
{
    std::cout << "Day20:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;
    std::cout << "    step1: " << step1(input_file) << "\n";
    std::cout << "    step2: " << step2(input_file) << "\n\n";
}

bool Day20::sanity_check()
{
    uint64_t step1_expected = 20899048083289;
    uint64_t step2_expected = 273;
    bool ret = true;

    uint64_t step1_got = step1(provided_file);
    if (step1_got != step1_expected)
    {
        fprintf(stderr,
                "\n    step1 sanity check failed: expected %zu, got %zu\n",
                step1_expected, step1_got);
        ret = false;
    }

    uint64_t step2_got = step2(provided_file);
    if (step2_got != step2_expected)
    {
        fprintf(stderr,
                "\n    step2 sanity check failed: expected %zu, got %zu\n",
                step2_expected, step2_got);
        ret = false;
    }

    if (ret)
        std::cout << " OK\n";
    return ret;
}
