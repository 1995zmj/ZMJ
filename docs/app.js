"use strict";
const articles = [
  { title: "在平凡的一天里，留意一点光", category: "生活", date: "2026-10-11", summary: "示例文章：关于日常观察与记录。", paragraphs: ["这是一篇示例文章，你可以将它替换成自己的生活记录。", "从窗边的光、一顿饭，或一次散步开始，记下今天值得保存的片刻。"] },
  { title: "读完一本书之后", category: "阅读", date: "2026-10-10", summary: "示例文章：给阅读留下一点回声。", paragraphs: ["这是一篇示例文章，你可以在这里分享书籍与阅读感受。", "记录一个触动你的观点，再写下它与自己生活的联系。"] },
  { title: "给尚未想明白的事留一点时间", category: "思考", date: "2026-10-09", summary: "示例文章：把问题写下来，慢慢寻找答案。", paragraphs: ["这是一篇示例文章，供你替换为真实的思考记录。", "不必急着得出结论。先把问题、观察与尝试记下来，也是一种进展。"] }
];
let category = "全部";
const posts = document.querySelector("#posts");
const search = document.querySelector("#search");
const filters = document.querySelector(".filters");
const reader = document.querySelector("#reader");
const content = document.querySelector("#content");
function element(tag, text, className) {
  const node = document.createElement(tag);
  node.textContent = text;
  if (className) node.className = className;
  return node;
}
function openArticle(article) {
  content.replaceChildren(element("div", `${article.date} · ${article.category} · 示例文章`, "meta"), element("h2", article.title), ...article.paragraphs.map(text => element("p", text)));
  reader.showModal();
}
function render() {
  const query = search.value.trim().toLocaleLowerCase();
  const matches = articles.filter(article => (category === "全部" || category === article.category) && [article.title, article.summary, ...article.paragraphs].join(" ").toLocaleLowerCase().includes(query));
  posts.replaceChildren();
  for (const article of matches) {
    const button = element("button", "", "post");
    button.type = "button";
    button.append(element("div", `${article.date} · ${article.category} · 示例文章`, "meta"), element("h3", article.title), element("p", article.summary), element("span", "阅读文章 →", "more"));
    button.addEventListener("click", () => openArticle(article));
    posts.append(button);
  }
  if (!matches.length) posts.append(element("p", "没有找到匹配的文章，试试其他关键词。", "empty"));
  for (const button of filters.children) {
    const selected = button.textContent === category;
    button.classList.toggle("active", selected);
    button.setAttribute("aria-pressed", String(selected));
  }
}
for (const label of ["全部", ...new Set(articles.map(article => article.category))]) {
  const button = element("button", label);
  button.type = "button";
  button.addEventListener("click", () => { category = label; render(); });
  filters.append(button);
}
search.addEventListener("input", render);
document.querySelector("#count").textContent = String(articles.length);
document.querySelector(".close").addEventListener("click", () => reader.close());
reader.addEventListener("click", event => {
  const bounds = reader.getBoundingClientRect();
  if (event.target === reader && (event.clientX < bounds.left || event.clientX > bounds.right || event.clientY < bounds.top || event.clientY > bounds.bottom)) reader.close();
});
document.querySelector("#all").addEventListener("click", () => { category = "全部"; search.value = ""; render(); });
document.querySelector("#aboutLink").addEventListener("click", event => {
  event.preventDefault();
  content.replaceChildren(element("h2", "关于这个博客"), element("p", "日常之间，是一个记录生活、阅读与思考的个人博客。当前名称、简介和文章均为初始示例，可在仓库 docs/index.html 与 docs/app.js 中替换。"));
  reader.showModal();
});
render();
